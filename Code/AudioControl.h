#pragma once

#include "AudioControl.h"
#include <string>
#include "Windows.h"
#include "SDL.h"

using namespace std;

enum SoundType
{
	SFX_SND,
	VOICE_SND,
	MUSIC_SND,
	OTHER_SND,

};

void PlaySoundClip(SoundType SDNtype, string sound);