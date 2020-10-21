#include "framework.h"
#include <fstream>
#include "Debug.h"
#include "algorithm"
#include "FileControl.h"
#include "SDL.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "SDL_mixer.h"
#include "Shaders.h"
#include <sstream>
#include <commdlg.h>
#include <fstream>
#include <typeinfo>
#include "memory"
#include "IDcontrol.h"


#pragma region DO NOT MOVE
#include <stdio.h>  /* defines FILENAME_MAX */
#include <direct.h>
#define GetCurrentDir _getcwd
std::string GetCurrentWorkingDir(void) {
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}
#pragma endregion

//The baseline file path that leads to all saved/loaded files
const std::string assetPath = GetCurrentWorkingDir();

//Verifies whether the file is retrievable.
bool VerifyFileOrFolder(std::string filePath) 
{
	filePath = "/" + filePath;
	if (filePath != assetPath)
		filePath = assetPath + filePath;
	DWORD ftyp = GetFileAttributesA(filePath.c_str());
	if (ftyp != INVALID_FILE_ATTRIBUTES) 
	{
		//WriteDebug("File found: " + filePath);
		return true;
	}
	//WriteDebug("File not found: " + filePath);
	return false;
}

//Initializes file control.
void FileControlStartup()
{
	VerifyFileOrFolder(assetPath);
	WriteDebug("File Control Startup -- Successful");
}

//Writes the asset path for a file based on its type and name, then verifies it.
//REFACTOR:: Folder asset paths? 
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
			fileName += ".png";
			break;
		}
		case AudioFile:
		{
			path += "Sounds/";
			fileName += ".wav";

			break;
		}

		case LevelFile:
		{
			path += "Levels/";
			fileName += ".map";
			break;
		}
		case AnimFile:
		{
			path += "Animations/";
			fileName += ".anim";
			break;
		}
		case PrefabFile:
		{
			path += "Prefabs/";
			fileName += ".png";
			break;
		}
		case SaveFile:
		{
			path += "Saves/";
			fileName += ".png";
			break;
		}

		case VertexShaderFile:
		{
			path += "Shaders/Vertex/";
			fileName += ".glsl";
			break;
		}
		case FragmentShaderFile:
		{
			path += "Shaders/Fragment/";
			fileName += ".glsl";
			break;
		}

		case GeometryShaderFile:
		{
			path += "Shaders/Geometry/";
			fileName += ".glsl";
			break;
		}

		case ObjFile:
		{
			path += "Models/";
			fileName += ".obj";
			break;
		}
		case ObjTextureFile:
		{
			path += "Models/";
			fileName += ".txt";
			break;
		}
	}
	path += fileName;
	VerifyFileOrFolder(path);

	return path;
}

//Saves the active file to its intended destination.
void SaveActiveFile(FileType fileType, std::string fileName, std::string data)
{
	std::string filePath = FetchPath(fileType, fileName, true);
	WriteDebug(filePath);
	std::ofstream file;
	file.open(filePath + ".txt");
	file << data;
	file.close();
}

