#pragma once
#include "framework.h"
#include <fstream>
#include <string>
#include "SDL_mixer.h"
#include "Shaders.h"

const enum FileType 
{
	None,
	ImageFile,
	AudioFile,
	AnimFile,
	ModelFile,
	SaveFile,
	PrefabFile,
	LevelFile,
	VertexShaderFile,
	FragmentShaderFile,

};


void CreateNewFile(FileType fileType, std::string fileName);
void OverwriteFile(FileType fileType, std::string fileName);
unsigned char* LoadImageFile(FileType fileType, std::string fileName, int&, int&, int&);
Mix_Chunk* LoadGameAudioFile(std::string fileName);

bool VerifyFileOrFolder(std::string filePath);
std::string FetchPath(FileType fileType, std::string fileName, bool saving);
void FileControlStartup();

Shader LoadCustomShader(std::string, std::string);