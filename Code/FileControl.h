#pragma once
#include "framework.h"
#include <fstream>
#include <string>

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

};

using namespace std;

void CreateNewFile(FileType fileType, string fileName);
void OverwriteFile(FileType fileType, string fileName);
unsigned char* LoadImageFile(FileType fileType, string fileName);
void LoadAudioFile(FileType fileType, string fileName);

bool VerifyFileOrFolder(string filePath);
string FetchPath(FileType fileType, string fileName, bool saving);
void FileControlStartup();