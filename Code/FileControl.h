#pragma once
#include "framework.h"
#include <fstream>
#include <string>
#include "SDL_mixer.h"
#include "Shaders.h"
#include "Vector"
#include "array"
#include "iterator"
#include "Map.h"

const enum FileType 
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

struct MapTile
{
	glm::ivec2 mapPos;
	TileFeature northFeature = Empty, eastFeature = Empty, westFeature = Empty, southFeature = Empty;
	std::string tileModel;
	MapDirection modelOrientation = North; //1 = north, 2 east, 3 south, 4 west //REPLACE WITH DIRECTION ENUM IMMEDIATELY
	std::vector<std::string> functionTriggers;
	MapTile(glm::ivec2 pos, TileFeature n, TileFeature s, TileFeature e, TileFeature w, std::string modelFile, MapDirection orient, std::vector<std::string> functionNames)
		: mapPos(pos), northFeature(n), eastFeature(e), westFeature(w), southFeature(s),
		tileModel(modelFile), modelOrientation(orient), functionTriggers(functionNames) {};
	MapTile() {};
};


struct AnimEvent 
{
private:
	std::string functionName;
	std::string variables;
	unsigned int activationFrame;
public:
	AnimEvent(std::string data)
	{
		std::string word;
		char c1;
		std::istringstream buf(word);
		if (buf >> functionName >> c1 >> variables && c1 == '(') 
		{
			//WriteDebug("Anim Event created for function: " + functionName);
		}
	};
	AnimEvent() {};
};

struct AnimFrame 
{
private:
	glm::ivec2 textureChanges = {0,0};

public:
	glm::ivec2 GetTextureChanges() { return textureChanges; }
	void SetTextureChange(unsigned int textNum, unsigned int value) 
	{
		if (textNum == 1) textureChanges.x = value;
		else if(textNum ==2) textureChanges.y = value;
	}

	AnimFrame() {};
};

struct AnimData 
{
private:
	unsigned int length;
	unsigned int totalTextures;
	std::vector<AnimFrame> frames;
	std::vector<Texture> textureLookup;
	std::vector<AnimEvent> events;
public:
	AnimData(unsigned int l, unsigned int t, std::vector<AnimFrame> frameVec, std::vector<Texture> loadtextures, std::vector<AnimEvent> eve):
		length(l), totalTextures(t), frames(frameVec), textureLookup(loadtextures), events(eve){};
};


struct ModelDataChunk 
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Edge> edges;
	std::vector<Texture> textures;
	//std::vector<std::pair<int, int>> faces;
	ModelDataChunk(std::vector<Vertex>& verts, std::vector<unsigned int>& indices,
		std::vector<Edge>& edges, std::vector<Texture>& textures/*, std::vector<std::pair<int,int>> faces*/): 
	vertices(verts), indices(indices), edges(edges), textures(textures)/*, faces(faces)*/{};
	ModelDataChunk() {};
};
struct MapDataChunk
{
	glm::ivec2 mapSize;
	std::vector<MapTile> tileMap;
	MapDataChunk(unsigned int& xSize, unsigned int& ySize, std::vector<MapTile>& tiles)
		: mapSize(glm::ivec2(xSize,ySize)), tileMap(tiles) {};
	MapDataChunk() {};
};



void SaveActiveFile(FileType fileType, std::string fileName, std::string data);
unsigned char* LoadImageFile(FileType fileType, std::string fileName, int&, int&, int&);
Mix_Chunk* LoadGameAudioFile(std::string fileName);
AnimData LoadAnimData(std::string fileName);
//If in the future there is any issue with model data check this function first. It is highly likely that the data did not get transferred properly and the bug was missed
// due to it working at the time. 
ModelDataChunk Load3DModel(std::string fileName, FileType fileType);
MapDataChunk LoadMapData(std::string fileName);

bool VerifyFileOrFolder(std::string filePath);
std::string FetchPath(FileType fileType, std::string fileName, bool saving);
void FileControlStartup();

Shader LoadCustomShader(std::string, std::string, std::string);