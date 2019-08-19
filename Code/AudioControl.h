#pragma once

#include "AudioControl.h"
#include <string>

enum SoundType
{
	SFX_SND,
	VOICE_SND,
	MUSIC_SND,
	OTHER_SND,

};

void PlaySoundClip(SoundType SDNtype, string sound);