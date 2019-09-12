#pragma once

#include "AudioControl.h"
#include <string>
#include "Windows.h"
#include "SDL.h"
#include "SDL_mixer.h"


using namespace std;

enum SoundType
{
	SFX_SND,
	VOICE_SND,
	MUSIC_SND,
	OTHER_SND,

};

void AudioControlStartup();

void AudioControlQuit();


void PlaySoundClip(SoundType SDNtype, string sound);