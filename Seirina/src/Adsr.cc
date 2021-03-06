/* adsr.cc: Attack/Decay/Sustain/Release Envelops
 *
 * Copyright 2017, 2018 Vincent Damewood
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

#include <Seirina/Adsr.h>
#include <Seirina/SampleDuration.h>

namespace Seirina::Audio
{
	class AdsrEnvelopePrivate
	{
	public:
		AdsrEnvelopePrivate(SampleDuration a, SampleDuration d, AdsrTransform s, SampleDuration r)
			: attack(a), decay(d), sustain(s), release(r)
		{
		}
		SampleDuration attack;
		SampleDuration decay;
		AdsrTransform sustain;
		SampleDuration release;
	};

	AdsrEnvelope::AdsrEnvelope(
		SampleDuration newAttack,
		SampleDuration newDecay,
		AdsrTransform newSustain,
		SampleDuration newRelease)
		: p(new AdsrEnvelopePrivate(newAttack, newDecay, newSustain, newRelease))
	{
	}

	AdsrTransform AdsrEnvelope::GetTransform(SampleIndex position, SampleDuration duration)
	{
		duration -= p->release;
		if (position < p->attack)
			return static_cast<double>(position)/static_cast<double>(p->attack);
		else if (position < (p->attack+p->decay))
			return 1.0
				- static_cast<double>(position - p->attack)
				* (1.0 - p->sustain)
				/ static_cast<double>(p->decay);
		else if (position <= duration)
			return p->sustain;
		else
			return p->sustain - (static_cast<double>(position-duration)
				/ static_cast<double>(p->release));
	}
};
