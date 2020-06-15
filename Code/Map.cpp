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

MapDataChunk currMap;
std::vector<MapEntity> allEntities;
MapDataChunk mapDataPTR;

//Returns a movement vector based on a mapDirection.
glm::ivec3 GetMoveVector(MapDirection dir) 
{
	glm::ivec3 moveVec;
	switch (dir)
	{
	case(North):moveVec = northVec; break;
	case(South):moveVec = southVec; break;
	case(East):moveVec = eastVec; break;
	case(West):moveVec = westVec; break;
	case(Up): moveVec = upVec; break;
	case(Down): moveVec = downVec; break;

	}
	return moveVec;
}

struct IDFinder {

	IDFinder(unsigned int const& id) : ID(id) { }

	bool operator () (const MapEntity el) const { return el.ID == ID; }

private:
	unsigned int ID;
};

//Adds a player to the pool of MapEntities to allow for movement.
void PlayerStartup(glm::ivec3 startingPos, MapDirection dir)
{
	allEntities.push_back(MapEntity(startingPos, dir, 0));
}
//Checks to see if a given location is within map boundaries.
bool IsOnCurrentMap(glm::ivec3 loc) 
{
	if (loc.x > currMap.mapSize.x -1 || loc.x < 0) return false;
	else if (loc.y > currMap.mapSize.y - 1 || loc.y < 0) return false;
	else if (loc.z > currMap.mapSize.z - 1 || loc.z < 0) return false;
	else return true;
};
//Loads map data and performs PlayerStartup()
void MapStartup(std::string mapName) 
{
	currMap = LoadMapData(mapName);
	PlayerStartup(glm::ivec3(0), North);
}

//returns the data of a maptile
MapTile* GetTile(glm::ivec3 tileLoc)
{
	return &currMap.tileMap[tileLoc.y * currMap.mapSize.y + tileLoc.x];
}

//checks to see if a tile can be entered by an entity from its current location.
//TODO:: Downward movement check needs to take into account ladders and similar structures.
//TODO:: Insert falling function.
bool TileMovable(glm::ivec3 currPos, MapDirection moveDir)
{
	glm::ivec3 moveVec = GetMoveVector(moveDir);
	if (moveDir == Up) 
	{
		return true;
	}

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
	else 
	{
		if (GetTile(currPos + moveVec)->getTileFeature(Down) == Empty)
		{
			WriteDebug("No floor boss--should be fallin'.");
		}
		else if (GetTile(currPos + moveVec)->getTileFeature(Down) != Floor)
		{
			WriteDebug("Incorrect floor for tile at: " + vecToStr(currPos + moveVec));
			return false;
		}
		return true;
	}

}
//Updates all the entities on the map by a single frame.
//TODO:: Map editing during runtime.
void UpdateMap() 
{
	for (int i = 0; i < allEntities.size(); i++) 
	{
		allEntities[i].AdvanceEntityFrame();
	}
}

//Returns a mapEntity from the pool of all mapentities.
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

//Loads map data from a text-based map file.
MapDataChunk& LoadMapData(std::string fileName)
{
	std::ifstream mapFile;

	std::string loadstr = FetchPath(LevelFile, fileName, false);
	mapFile.open(loadstr);
	if (!mapFile.is_open())
	{
		WriteDebug("Cannot Open File: " + fileName);
	}
	else
		WriteDebug("Loading File..." + fileName);
	unsigned int x, y, z;
	std::vector<MapTile> tiles;

	for (std::string line; std::getline(mapFile, line);)
	{
		std::istringstream in(line);
		std::string type;
		in >> type;

		int orient;
		TileFeatures features;
		std::string tileModel;
		std::vector<std::string> functionTriggers;

		if (type == "s")
		{
			int testX, testY, testZ;
			in >> testX >> testY >> testZ;
			if (testX <= 0 || testY <= 0 || testZ <=0)
				WriteDebug("File Error: Map Size incorrect");
			else { x = testX; y = testY; z = testZ; }
		}
		if (type == "t")
		{
			std::istringstream iss(line);
			std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
				std::istream_iterator<std::string>());
			features.north = (TileFeature)std::stoi(results[1]);
			features.east = (TileFeature)std::stoi(results[2]);
			features.south = (TileFeature)std::stoi(results[3]);
			features.west = (TileFeature)std::stoi(results[4]);
			features.upward = (TileFeature)std::stoi(results[5]);
			features.downward = (TileFeature)std::stoi(results[6]);

			tileModel = results[7];
			orient = std::stoi(results[8]);
			if (results.size() > 9)
			{
				for (int i = 9; i < results.size(); i++)
				{
					functionTriggers.push_back(results[i]);
				}
			}
			tiles.push_back(MapTile(glm::ivec3(x, y, z),
				features, tileModel, (MapDirection)orient, functionTriggers));
		}
	}

	mapDataPTR = MapDataChunk(x, y, z, tiles);
	mapFile.close();
	return mapDataPTR;
};