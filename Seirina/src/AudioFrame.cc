/* AudioFrame.cc: Collection of audio samples
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

#include <vector>

#include <Seirina/AudioFrame.h>

using Seirina::Audio::Sample;

namespace Seirina::Audio
{
	class FramePrivate
	{
	public:
		FramePrivate(size_t size)
		{
			channels.reserve(size);
		}
		std::vector<Sample> channels;
	};

	Frame::Frame(Sample input)
		: p(new FramePrivate(1))
	{
		p->channels.push_back(input);
	}

	Frame::Frame(Sample left, Sample right)
		: p(new FramePrivate(2))
	{
		p->channels.push_back(left);
		p->channels.push_back(right);
	}

	Frame::~Frame()
	{
		delete p;
	}

	Sample Frame::operator[] (int idx) const
	{
		return p->channels[idx];
	}
};
