#include "Debug.h"
#include "algorithm"
#include "SDL.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "Map.h"
#include <glm/glm.hpp>
#include "Vector"
#include "array"
#include "iterator"
#include "FileControl.h"
#include "MapEntity.h"

MapDataChunk currMap;
MapEntity testEntity;

void MapStartup(std::string mapName) 
{
	currMap = LoadMapData(mapName);
	testEntity = MapEntity();
}

void TestMove()
{
	testEntity.Move(North);
}


MapTile* GetTile(glm::ivec2 tileLoc)
{
	return &currMap.tileMap[tileLoc.y * currMap.mapSize.y + tileLoc.x];
}

bool TileMovable(glm::ivec2 currPos, MapDirection moveDir)
{
	if (GetTile(currPos)->getTileFeature(moveDir) == Wall)
		return false;
	else return true;
}