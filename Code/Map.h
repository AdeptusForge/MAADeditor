#pragma once

#include "framework.h"
#include "Debug.h"
#include "algorithm"
#include "SDL.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Vector"
#include "array"
#include "iterator"

enum MapDirection 
{
	North = 1,
	East = 2,
	South = 3,
	West = 4
};

enum TileFeature 
{
	Empty = 0,
	Wall = 1,

};

struct MapTile
{
private:
	glm::ivec2 mapPos;
	glm::ivec4 tileFeatures;
	std::string tileModel;
	MapDirection modelOrientation = North; //1 = north, 2 east, 3 south, 4 west //REPLACE WITH DIRECTION ENUM IMMEDIATELY
	std::vector<std::string> functionTriggers;
public:
	MapTile(glm::ivec2 pos, glm::ivec4 f, std::string modelFile, MapDirection orient, std::vector<std::string> functionNames)
		: mapPos(pos), tileFeatures(f), tileModel(modelFile), modelOrientation(orient), functionTriggers(functionNames) {};
	MapTile() {};
	TileFeature getTileFeature(MapDirection dir)
	{
		if(dir == North)
			return (TileFeature)tileFeatures.w;
		else if(dir == East)
			return (TileFeature)tileFeatures.x;
		else if (dir == South)
			return (TileFeature)tileFeatures.y;
		else if (dir == West)
			return (TileFeature)tileFeatures.z;
	}
	std::string getTileModel() { return tileModel; }

};

struct MapDataChunk
{
	glm::ivec2 mapSize;
	std::vector<MapTile> tileMap;
	MapDataChunk(unsigned int& xSize, unsigned int& ySize, std::vector<MapTile>& tiles)
		: mapSize(glm::ivec2(xSize, ySize)), tileMap(tiles) {};
	MapDataChunk() {};
};

void MapStartup(std::string mapName);

void TestMove();
MapTile* GetTile(glm::ivec2 tileLoc);

bool TileMovable(glm::ivec2 currPos, MapDirection moveDir);
