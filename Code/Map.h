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

const glm::ivec2 northVec = glm::ivec2(0,1);
const glm::ivec2 eastVec = glm::ivec2(1, 0);
const glm::ivec2 southVec = glm::ivec2(0, -1);
const glm::ivec2 westVec = glm::ivec2(-1, 0);


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

glm::ivec2 GetMoveVector(MapDirection dir);

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

bool IsOnCurrentMap(glm::ivec2 loc);
void MapStartup(std::string mapName);
MapTile* GetTile(glm::ivec2 tileLoc);
bool TileMovable(glm::ivec2 currPos, MapDirection moveDir);

//ID will be used to determine fixed-location encounters and when a MapEntity is the player(ID = 0).
class MapEntity
{
private:
	glm::ivec2 currentMapPos;
	MapDirection currentFacing;
public:
	glm::ivec2 GetCurrentPos() { return currentMapPos; }
	//1 = CurrentFacing, 2 = CurrentFacing turned right once, 3 = Opposite of CurrentFacing, 4 = CurrentFacing turned left once
	MapDirection GetCurrentFacing(unsigned int check) 
	{
		unsigned int checkFace = currentFacing;
		switch (check) 
		{
			case(2): 
			{
				if (currentFacing == West)
					checkFace = 1;
				else
					checkFace += 1;
				break;
			}
			case(3):
			{
				if (checkFace >= 3) {
					checkFace -= 4;
				}
				checkFace += 2;
				break;
			}
			case(4):
			{
				if (currentFacing == North)
					checkFace = 4;
				else
					checkFace -= 1;
				break;
			}
		}
		return (MapDirection)checkFace;
	}
	unsigned int ID;

	//Left = 1, Right = 0
	void Rotate(bool dir)
	{
		int newFacing = currentFacing;
		//Turn Right
		if (!dir)
		{
			WriteDebug("Rotated right" + std::to_string(ID));
			if (currentFacing == West)
				newFacing = 1;
			else
				newFacing += 1;
			//IF PLAYER, ROTATE CAMERA
		}
		//Turn Left
		else
		{
			WriteDebug("Rotated left" + std::to_string(ID));

			if (currentFacing == North)
				newFacing = 4;
			else
				newFacing -= 1;

			//IF PLAYER, ROTATE CAMERA
		}
		ChangeFacing((MapDirection)newFacing);
	}
	void Flip()
	{
		WriteDebug("Flipped " + std::to_string(ID));
		int currentDir = currentFacing;
		if (currentDir >= 3) {
			currentDir -= 4;
		}
		currentDir += 2;
		//IF PLAYER, ROTATE CAMERA
		ChangeFacing((MapDirection)currentDir);
	}
	void ChangeFacing(MapDirection dir)
	{
		currentFacing = dir;
	}
	void Walk(MapDirection dir)
	{
		MapDirection moveDir =dir;
		if (currentFacing != moveDir) 
		{
			//IF PLAYER, MOVE CAMERA
		}

		if (TileMovable(GetCurrentPos(), moveDir))
		{
			WriteDebug("Can Move " + std::to_string(moveDir) + " from tile: " + vecToStr(GetCurrentPos()));
			//IF PLAYER, MOVE CAMERA
			currentMapPos += GetMoveVector(moveDir);
			WriteDebug("New Position: " + vecToStr(GetCurrentPos()));
		}
		else
			WriteDebug("Can't Move " + std::to_string(moveDir) + " from tile: " + vecToStr(GetCurrentPos()));
	}

	MapEntity(glm::ivec2 startingPos, MapDirection startingFacing, unsigned int entityID) :
		currentMapPos(startingPos), currentFacing(startingFacing), ID(entityID) {};
};

void PlayerStartup(glm::ivec2 startingPos, MapDirection dir);
MapEntity* GetMapEntity(unsigned int entityID);