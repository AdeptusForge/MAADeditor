#include "framework.h"
#include <fstream>
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

unsigned char* imageData;

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
			if (!saving)
				fileName += ".txt";
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
			path += "Shaders/Vertex/";
			if (!saving)
				fileName += ".glsl";
			break;
		}
		case FragmentShaderFile:
		{
			path += "Shaders/Fragment/";
			if (!saving)
				fileName += ".glsl";
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

unsigned char* LoadImageFile(FileType fileType, std::string fileName, int &width, int &height, int &nrChannels)
{
	stbi_set_flip_vertically_on_load(true);
	std::string loadstr = FetchPath(ImageFile, fileName, false);
	imageData = stbi_load(loadstr.c_str(), &width, &height, &nrChannels, 0);
	if(!imageData)
		WriteDebug(stbi_failure_reason());
	return imageData;
}

Mix_Chunk* LoadGameAudioFile(std::string fileName)
{
	std::string loadstr = FetchPath(AudioFile, fileName, false);

	Mix_Chunk *sample = Mix_LoadWAV(loadstr.c_str());
	if (sample == NULL) WriteDebug("Sound not found: " + std::string(loadstr));

	return sample;
}

ModelDataChunk* Load3DModel(std::string fileName)
{
	std::ifstream modelFile;
	std::string loadstr = FetchPath(ModelFile, fileName, false);
	modelFile.open(loadstr);
	if (!modelFile.is_open())
	{
		exit(EXIT_FAILURE);
	}

	std::vector<std::string> texturePaths;
	std::vector<Texture> textures;
	//texturePaths =


	std::vector<Vertex> vertices;
	
	std::vector<unsigned int> indices;
	std::vector<Edge> edges;


	
	ModelDataChunk* newModel;
	newModel->vertices = vertices;
	newModel->indices = indices;
	newModel->edges = edges;
	newModel->textures = textures;


	modelFile.close();
	return newModel;

}

Shader LoadCustomShader(std::string vertexPath, std::string fragmentPath)
{
	vertexPath = FetchPath(VertexShaderFile, vertexPath, false);
	fragmentPath = FetchPath(FragmentShaderFile, fragmentPath, false);

	const GLchar* newVertexPath = vertexPath.c_str();
	const GLchar* newFragmentPath = fragmentPath.c_str();
	Shader ourShader(newVertexPath, newFragmentPath);
	return ourShader;
}