#pragma once
#include "framework.h"
#include <fstream>
#include <string>
#include "SDL_mixer.h"
#include "Vector"
#include "array"
#include "iterator"
#include "IDcontrol.h"


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

bool VerifyFileOrFolder(std::string filePath);
std::string FetchPath(FileType fileType, std::string fileName, bool saving);
void FileControlStartup();