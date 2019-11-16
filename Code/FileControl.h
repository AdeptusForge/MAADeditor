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
public:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoords;
	Vertex(glm::vec3 pos, glm::vec3 col, glm::vec2 tex) : position(pos), color(col), texCoords(tex) {};
};
struct Edge
{
	glm::vec3 edgeStart;
	glm::vec3 edgeEnd;
	glm::vec3 color;
	Edge(glm::vec3 start, glm::vec3 end, glm::vec3 col): edgeStart(start), edgeEnd(end), color(col) {};
};
struct Texture
{
	unsigned int ID;
	std::string name;
	Texture(unsigned int id, std::string name) : ID(id), name(name) {};
};
struct ModelDataChunk 
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Edge> edges;
	std::vector<Texture> textures;
	ModelDataChunk(std::vector<Vertex>& verts, std::vector<unsigned int>& indices,
		std::vector<Edge>& edges, std::vector<Texture>& textures): 
	vertices(verts), indices(indices), edges(edges), textures(textures)
	{};
	ModelDataChunk() {};
};



void SaveActiveFile(FileType fileType, std::string fileName, std::string data);
unsigned char* LoadImageFile(FileType fileType, std::string fileName, int&, int&, int&);
Mix_Chunk* LoadGameAudioFile(std::string fileName);
//If in the future there is any issue with model data check this function first. It is highly likely that the data did not get transferred properly and the bug was missed
// due to it working at the time. 
ModelDataChunk Load3DModel(std::string fileName);

bool VerifyFileOrFolder(std::string filePath);
std::string FetchPath(FileType fileType, std::string fileName, bool saving);
void FileControlStartup();

Shader LoadCustomShader(std::string, std::string);