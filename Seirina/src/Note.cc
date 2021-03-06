/* Note.cc: Musical note
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

#include <cmath>
#include <map>

#include <Seirina/Note.h>
#include <Seirina/PitchClass.h>


using Seirina::Notation::Octave;
using Seirina::Notation::PitchClass;

namespace Seirina::Notation
{
	class NotePrivate
	{
	public:
		NotePrivate(
			PitchClass newPitchClass,
			Octave newOctave,
			Duration newDuration)
		: pitchClass(newPitchClass),
			octave(newOctave),
			duration(newDuration)
		{
		}

		PitchClass pitchClass;
		Octave octave;
		Duration duration;
	};

	Note::Note(
			enum PitchClass newPitchClass,
			class Octave newOctave,
			class Duration newDuration)
		: p(new NotePrivate(
			newPitchClass,
			newOctave,
			newDuration))
	{
	}

	Note::Note(const Note& src)
		: p(new NotePrivate(
			src.p->pitchClass,
			src.p->octave,
			src.p->duration))
	{
	}

	Note::~Note()
	{
		//delete p;
	}

	const PitchClass& Note::PitchClass() const
	{
		return p->pitchClass;
	}

	const Octave& Note::Octave() const
	{
		return p->octave;
	}

	Seirina::Audio::Frequency Note::Frequency(Tuning inTuning) const
	{
		return inTuning[p->pitchClass] * std::pow(2.0, p->octave - 4);
	}


	Duration Note::Duration() const
	{
		return p->duration;
	}
};
