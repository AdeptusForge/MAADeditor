#pragma once
#include "framework.h"
#include <fstream>
#include <string>
#include "SDL_mixer.h"
#include "Shaders.h"
#include "Vector"

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



//model bits
struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoords;
};
struct Edge
{
	glm::vec3 edgeStart;
	glm::vec3 edgeEnd;
	glm::vec3 color;
};
struct Texture
{
	unsigned int ID;
	std::string name;
};
struct ModelDataChunk 
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Edge> edges;
	std::vector<Texture> textures;

};



void CreateNewFile(FileType fileType, std::string fileName);
void OverwriteFile(FileType fileType, std::string fileName);
unsigned char* LoadImageFile(FileType fileType, std::string fileName, int&, int&, int&);
Mix_Chunk* LoadGameAudioFile(std::string fileName);
ModelDataChunk* Load3DModel(std::string fileName);

bool VerifyFileOrFolder(std::string filePath);
std::string FetchPath(FileType fileType, std::string fileName, bool saving);
void FileControlStartup();

Shader LoadCustomShader(std::string, std::string);