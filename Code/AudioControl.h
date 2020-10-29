#pragma once

#include "AudioControl.h"
#include <string>
#include "Windows.h"
#include "SDL.h"
#include "SDL_mixer.h"

enum SoundType
{
	SFX_SND,
	VOICE_SND,
	MUSIC_SND,
	OTHER_SND,
};

void AudioControlStartup();
void AudioControlQuit();
void PlaySoundClip(SoundType SDNtype, std::string sound);
Mix_Chunk* LoadGameAudioFile(std::string fileName);