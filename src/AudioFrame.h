/* AudioFrame.cc: Representation of momentary audio sample
 *
 * Copyright 2016, 2018 Vincent Damewood
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

#if !defined AUDIO_FRAME_H
#define AUDIO_FRAME_H

class AudioFrame
{
public:
	AudioFrame(double); // mono
	AudioFrame(double, double); //stereo

	~AudioFrame();

	double Left() const;
	double Right() const;
	AudioFrame Transform(double transformation) const;
private:
	class Pimpl;
	Pimpl* p;
};

#endif /* AUDIO_FRAME_H */
