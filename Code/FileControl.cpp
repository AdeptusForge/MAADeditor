#include "framework.h"
#include <fstream>
#include "Assets.h"
#include "Debug.h"
#include "FileControl.h"
#include <fstream>
#include "SDL.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "SDL_mixer.h"
#include "Shaders.h"


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



//The baseline file path that leads to all saved/loaded files
const std::string assetPath = GetCurrentWorkingDir();

bool VerifyFileOrFolder(std::string filePath) 
{
	filePath = "/" + filePath;
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

std::string FetchPath(FileType fileType, std::string fileName, bool saving)
{
	std::string path;
	path += "Assets/";

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

		case VertexShaderFile:
		{
			path += "Shaders/";
			if (!saving)
				fileName += ".vshad";
			break;
		}
		case FragmentShaderFile:
		{
			path += "Shaders/";
			if (!saving)
				fileName += ".fshad";
			break;
		}
	}
	if (!saving)
		path += fileName;
	VerifyFileOrFolder(path);

	return path;
}

void CreateNewFile(FileType fileType, std::string fileName)
{
	
}
void SaveOverFile(FileType fileType, std::string fileName)
{

}
unsigned char* LoadImageFile(FileType fileType, std::string fileName)
{
	int width = 800, height = 800, nrChannels;
	const char* loadstr = FetchPath(ImageFile, fileName, false).c_str();

	unsigned char* data = stbi_load(loadstr, &width, &height, &nrChannels, 0);
	return data;
}

Mix_Chunk* LoadGameAudioFile(std::string fileName)
{
	const char* loadstr = FetchPath(AudioFile, fileName, false).c_str();
	Mix_Chunk *sample = Mix_LoadWAV(loadstr);
	if (sample == NULL) WriteDebug("Sound not found: " + std::string(loadstr));

	return sample;
}

//Loads shaders from a path;
Shader LoadCustomShader(std::string vertexPath, std::string fragmentPath)
{
	const GLchar* newVertexPath = FetchPath(VertexShaderFile, vertexPath, false).c_str();
	const GLchar* newFragmentPath = FetchPath(FragmentShaderFile, fragmentPath, false).c_str();
	Shader ourShader(newVertexPath, newFragmentPath);
	return ourShader;
}