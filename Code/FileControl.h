#pragma once
#include "framework.h"
#include <fstream>
#include <string>
#include "SDL_mixer.h"
#include "Shaders.h"
#include "Vector"
#include "array"
#include "iterator"

#ifndef MAX_MODEL_TEXTURES
const unsigned int MAX_MODEL_TEXTURES = 8;
#endif // !MAX_MODEL_TEXTURES

enum FileType 
{
	None,
	ImageFile,
	AudioFile,
	AnimFile,
	SaveFile,
	PrefabFile,
	LevelFile,
	VertexShaderFile,
	FragmentShaderFile,
	GeometryShaderFile,
	ObjFile,
	ObjTextureFile
};






void SaveActiveFile(FileType fileType, std::string fileName, std::string data);

//If in the future there is any issue with model data check this function first. It is highly likely that the data did not get transferred properly and the bug was missed
// due to it working at the time. 


bool VerifyFileOrFolder(std::string filePath);
std::string FetchPath(FileType fileType, std::string fileName, bool saving);
void FileControlStartup();