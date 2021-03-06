/* InputParser.cc: Parser for music input files
 *
 * Copyright 2018, 2019 Vincent Damewood
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <fstream>

#include <Seirina/PitchClass.h>

#include "InputParser.h"

using Seirina::Notation::PitchClass;
using Seirina::Notation::MakePitchClass;
using Seirina::Notation::Note;
using Seirina::Notation::Rest;
using Seirina::Notation::Duration;

class ParserToken::Pimpl
{
public:
	Pimpl() {};
	Pimpl(std::unique_ptr<Note> newNote) {note = std::move(newNote);}
	Pimpl(std::unique_ptr<Rest> newRest) {rest = std::move(newRest);}
	std::unique_ptr<Note> note = nullptr;
	std::unique_ptr<Rest> rest = nullptr;
};

ParserToken::ParserToken ()
	: p(new Pimpl){};

ParserToken::ParserToken (std::unique_ptr<Note> newNote)
	: p(new Pimpl(std::move(newNote))){};

ParserToken::ParserToken (std::unique_ptr<Rest> newRest)
	: p(new Pimpl(std::move(newRest))){};

ParserToken::~ParserToken()
{
	delete(p);
}

bool ParserToken::IsNote()
{
	return p->note != nullptr;
}

bool ParserToken::IsRest()
{
	return p->rest != nullptr;
}

std::unique_ptr<Note> ParserToken::ExtractNote()
{
	return std::move(p->note);
}

std::unique_ptr<Rest> ParserToken::ExtractRest()
{
	return std::move(p->rest);
}

class InputParserPrivate
{
public:
	std::ifstream* File;
};

InputParser::InputParser(const char* Filename)
{
	d = new InputParserPrivate();
	d->File = new std::ifstream(Filename);
}

InputParser::~InputParser()
{
	d->File->close();
	delete d->File;
	delete d;
}

ParserToken InputParser::Fetch()
{
	char NoteLetter = ' ';
	char NoteAccidental = ' ';
	int NoteOctave = 4;
	int DurationDenominator = 1;
	int DurationNumerator = 1;

	int inChar = d->File->get();

	if (inChar == EOF)
		return ParserToken();


	if (inChar == '=')
	{
		while((inChar = d->File->get()) != '\n');
		return Fetch();
	}


	switch (inChar)
	{
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'R':
		NoteLetter = inChar;
		break;
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'r':
		NoteLetter = inChar - ('a' - 'A');
		break;
	default:
		return ParserToken();
	}

	if (NoteLetter != 'R')
	{
		switch (inChar = d->File->get())
		{
			case '-':
			case '+':
			case 'b':
			case '#':
			case ' ':
				NoteAccidental = inChar;
				inChar = d->File->get();
		}


		NoteOctave = inChar - '0';
		if (inChar < '0' || inChar > '9')
			return ParserToken();
		NoteOctave = inChar - '0';
	}

	inChar = d->File->get();
	if (inChar != '-')
		return ParserToken();


	inChar = d->File->get();
	if (inChar < '0' || inChar > '9')
		return ParserToken();
	DurationNumerator = inChar - '0';


	inChar = d->File->get();
	if (inChar == '/')
	{
		inChar = d->File->get();
		if (inChar < '1' || inChar > '9')
			return ParserToken();
		DurationDenominator = inChar - '0';
		inChar = d->File->get();
	}


	if (inChar == '\r')
		inChar = d->File->get();
	if (inChar != '\n')
		return ParserToken();

	if (NoteLetter != 'R')
		return ParserToken(std::unique_ptr<Note>(new Note(
			MakePitchClass(NoteLetter, NoteAccidental),
			NoteOctave, Duration(
			DurationNumerator,
			DurationDenominator))));
	else
		return ParserToken(std::unique_ptr<Rest>(new Rest(
			Duration(
				DurationNumerator,
				DurationDenominator))));
}
