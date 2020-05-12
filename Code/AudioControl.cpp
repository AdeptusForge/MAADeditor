#include <string>
#include <fstream>
#include "Windows.h"
#include "Debug.h"
#include "AudioControl.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "FileControl.h"

unsigned int mainVolume;
unsigned int sfxVolume;
unsigned int voiceVolume;
unsigned int musicVolume;

//Loads a .wav music file into SDL Mix_Chunk
Mix_Chunk* LoadGameAudioFile(std::string fileName)
{
	std::string loadstr = FetchPath(AudioFile, fileName, false);

	Mix_Chunk* sample = Mix_LoadWAV(loadstr.c_str());
	if (sample == NULL) WriteDebug("Sound not found: " + std::string(loadstr));

	return sample;
}

//A function for returning special volume modifiers depending on the situation.
int SpecialSoundModifier(std::string sound) 
{
	return 100;
}

//Returns overall sound volume by factoring in all possible sound modifiers, including options.
float SoundVolume(SoundType SNDtype, std::string sound/*include sound distance/direction identifier here*/) 
{
	float volumePercentage = 0;
	switch (SNDtype)
	{
		case SFX_SND: 
		{
			volumePercentage = (sfxVolume / 100);
			break;
		}
		case VOICE_SND:
		{
			volumePercentage =(voiceVolume / 100);
			break;
		}
		case MUSIC_SND:
		{
			volumePercentage = (musicVolume / 100);
			break;
		}
		case OTHER_SND:
		{
			volumePercentage = (SpecialSoundModifier(sound)/100);
			break;
		}
	}
	volumePercentage *= (mainVolume/100);
	WriteDebug("VolumeCheck: " + std::to_string(volumePercentage));
	return volumePercentage;
}

//Plays a loaded soundclip.
void PlaySoundClip(SoundType SNDtype, std::string sound) 
{
	switch (SNDtype) 
	{
		case(SFX_SND):
			Mix_PlayChannel(-1, LoadGameAudioFile(sound), 1);
	}
}

//Initialization of Audio Mixer
void AudioControlStartup()
{

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		WriteDebug("Audio Control Startup -- Failed");
	WriteDebug("Audio Control Startup -- Successful");
}

//Shuts down Audio Mixer
void AudioControlQuit() 
{
	Mix_Quit();
}