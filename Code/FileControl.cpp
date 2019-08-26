#include "framework.h"
#include <fstream>
#include "Assets.h"
#include "Debug.h"
#include "FileControl.h"
#include <fstream>
#include "SDL.h"
#include <GLFW/glfw3.h>
#include "SDL_mixer.h"



//MUST be after all other #includes, and can only exist in 1 file. DO NOT MOVE
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#include <direct.h>
#define GetCurrentDir _getcwd
std::string GetCurrentWorkingDir(void) {
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}



using namespace std;
//The baseline file path that leads to all saved/loaded files
const string assetPath = GetCurrentWorkingDir() + "/Assets/";

bool VerifyFileOrFolder(string filePath) 
{
	if (filePath != assetPath)
		filePath = assetPath + filePath;
	DWORD ftyp = GetFileAttributesA(filePath.c_str());
	if (ftyp != INVALID_FILE_ATTRIBUTES) 
	{
		WriteDebug("File found: " + filePath);
		return true;
	}
	WriteDebug("File not found: " + filePath);
	return false;
}

void FileControlStartup()
{
	VerifyFileOrFolder(assetPath);
	WriteDebug("File Control Startup -- Successful");
}

string FetchPath(FileType fileType, string fileName, bool saving)
{
	string path;
	switch (fileType)
	{
		case None:
		{
			break;
		}
		case ImageFile:
		{
			path += "Textures/";
			if (!saving)
				fileName += ".png";
			break;
		}
		case AudioFile:
		{
			path += "Sounds/";
			if (!saving)
				fileName += ".wav";

			break;
		}
		case ModelFile:
		{
			path += "Models/";
			/*if (!saving)
				fileName += ".png";*/
			break;
		}
		case LevelFile:
		{
			path += "Levels/";
			/*if (!saving)
				fileName += ".png";*/
			break;
		}
		case AnimFile:
		{
			path += "Animations/";
			/*if (!saving)
				fileName += ".png";*/
			break;
		}
		case PrefabFile:
		{
			path += "Prefabs/";
			/*if (!saving)
				fileName += ".png";*/
			break;
		}
		case SaveFile:
		{
			path += "Saves/";
			/*if (!saving)
				fileName += ".png";*/
			break;
		}
	}
	if (!saving)
		path += fileName;
	VerifyFileOrFolder(path);
	
	return path;
}

void CreateNewFile(FileType fileType, string fileName) 
{
	
}
void SaveOverFile(FileType fileType, string fileName)
{

}
unsigned char* LoadImageFile(FileType fileType, string fileName)
{
	int width = 800, height = 800, nrChannels;
	const char* loadstr = FetchPath(ImageFile, fileName, false).c_str();

	unsigned char* data = stbi_load(loadstr, &width, &height, &nrChannels, 0);
	return data;
}

void LoadAudioFile(FileType fileType, string fileName)
{
	SDL_AudioSpec wavSpec;
	Uint8* wavStart;
	Uint32 wavLength;
	const char* loadstr = FetchPath(AudioFile, fileName, false).c_str();

	SDL_LoadWAV(loadstr, &wavSpec, &wavStart, &wavLength);
}