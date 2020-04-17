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

MapDataChunk currMap;
std::vector<MapEntity> allEntities;

glm::ivec2 GetMoveVector(MapDirection dir) 
{
	glm::ivec2 moveVec;
	switch (dir)
	{
	case(North):moveVec = northVec; break;
	case(South):moveVec = southVec; break;
	case(East):moveVec = eastVec; break;
	case(West):moveVec = westVec; break;
	}
	return moveVec;
}

struct IDFinder {

	IDFinder(unsigned int const& id) : ID(id) { }

	bool operator () (const MapEntity el) const { return el.ID == ID; }

private:
	unsigned int ID;
};

void PlayerStartup(glm::ivec2 startingPos, MapDirection dir)
{
	allEntities.push_back(MapEntity(startingPos, dir, 0));
}
bool IsOnCurrentMap(glm::ivec2 loc) 
{
	if (loc.x > currMap.mapSize.x -1 || loc.x < 0) return false;
	else if (loc.y > currMap.mapSize.y - 1 || loc.y < 0) return false;
	else return true;
};

void MapStartup(std::string mapName) 
{
	currMap = LoadMapData(mapName);
	PlayerStartup(glm::ivec2(0), North);
}

MapTile* GetTile(glm::ivec2 tileLoc)
{
	return &currMap.tileMap[tileLoc.y * currMap.mapSize.y + tileLoc.x];
}

bool TileMovable(glm::ivec2 currPos, MapDirection moveDir)
{
	glm::ivec2 moveVec = GetMoveVector(moveDir);
	if (GetTile(currPos)->getTileFeature(moveDir) == Wall) 
	{
		WriteDebug("Obstructing TileFeature");
		return false;
	}
	else if (!IsOnCurrentMap(currPos + moveVec)) 
	{
		WriteDebug("We're off the edge of the world chief!");
		return false;
	}
	else return true;
}

MapEntity* GetMapEntity(unsigned int entityID) 
{
	if (!allEntities.empty())
	{
		MapEntity* p;
		std::vector<MapEntity>::iterator it = std::find_if(allEntities.begin(), allEntities.end(), IDFinder(entityID));
		if (!std::none_of(allEntities.begin(), allEntities.end(), IDFinder(entityID)))
			p = &(*it);
		else
		{
			WriteDebug("No RenderObject with matching ID");
			return nullptr;
		}
		return p;

	}
	WriteDebug("No AllModels");
	return nullptr;
}