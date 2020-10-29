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
#include "camera.h"
#include "render.h"
#include "Objects.h"

#pragma region const variables
#ifndef PLAYER_ENTITYID
const unsigned int PLAYER_ENTITYID = 0;
#endif // !PLAYER_ENTITYID

#ifndef northVec
const glm::ivec3 northVec = glm::ivec3(0, 1, 0);
#endif // !northVec

#ifndef eastVec
const glm::ivec3 eastVec = glm::ivec3(1, 0, 0);
#endif // !eastVec

#ifndef southVec
const glm::ivec3 southVec = glm::ivec3(0, -1, 0);
#endif // !southVec

#ifndef westVec
const glm::ivec3 westVec = glm::ivec3(-1, 0, 0);
#endif // !westVec

#ifndef upVec
const glm::ivec3 upVec = glm::ivec3(0, 0, 1);
#endif // !upVec

#ifndef downVec
const glm::ivec3 downVec = glm::ivec3(0, 0, -1);
#endif // !downVec

#ifndef northQuat
const glm::quat northQuat = glm::radians(glm::vec3(0, 0, 0) * 0.5f);
#endif // !northQuat
#ifndef eastQuat
const glm::quat eastQuat = glm::radians(glm::vec3(0, -90, 0) * 0.5f);
#endif // !eastQuat
#ifndef southQuat
const glm::quat southQuat = glm::radians(glm::vec3(0, 180, 0) * 0.5f);
#endif // !southQuat
#ifndef westQuat
const glm::quat westQuat = glm::radians(glm::vec3(0, 90, 0) * 0.5f);
#endif // !westQuat
#pragma endregion



const std::vector<glm::vec3> walkForward = 
{
	glm::vec3(0.0, 0.0, 0.1), 
	glm::vec3(0.0, 0.0, 0.1),
	glm::vec3(0.0, 0.0, 0.1),
	glm::vec3(0.0, 0.0, 0.1),
	glm::vec3(0.0, 0.0, 0.1),
	glm::vec3(0.0, 0.0, 0.1),
	glm::vec3(0.0, 0.0, 0.1),
	glm::vec3(0.0, 0.0, 0.1),
	glm::vec3(0.0, 0.0, 0.1),
	glm::vec3(0.0, 0.0, 0.1),
};

glm::ivec3 GetMoveVector(MapDirection dir);

bool IsOnCurrentMap(glm::ivec3 loc);
void MapStartup(std::string mapName);
MapTile* GetTile(glm::ivec3 tileLoc);
bool TileMovable(glm::ivec3 currPos, MapDirection moveDir);

//Essentially anything that moves of its own accord on a map within the game.
//ID is used to determine fixed-location encounters and when a MapEntity is the player(ID = 0).
class MapEntity
{
private:
	glm::ivec3 currentMapPos;
	MapDirection currentFacing;
	unsigned int actionFrame;
	unsigned int currMaxFrame = 0;

public:
	glm::ivec3 GetCurrentPos() { return currentMapPos; }
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

	glm::vec3 RotateCoords(glm::vec3 check)
	{
		glm::quat checkQuat;

		switch (currentFacing)
		{
		case North: {checkQuat = northQuat; break; }
		case East: {checkQuat = eastQuat; break; }
		case South: {checkQuat = southQuat; break; }
		case West: {checkQuat = westQuat; break; }
		}

		glm::vec3 rotatedPoint = checkQuat * check * glm::inverse(checkQuat);

		//WriteDebug("Rotated: " + vecToStr(rotatedPoint) + std::to_string(currentFacing));

		rotatedPoint = check;

		return rotatedPoint;
	}


	//Mostly to keep the player from being able to move before he's supposed to.
	void AdvanceEntityFrame() 
	{
		//WriteDebug(actionFrame);
		actionFrame++;
		if (actionFrame > currMaxFrame) 
		{
			actionFrame = 0;
			currMaxFrame = 0;
			//WriteDebug("End of the last Action");
		}
	}

	//Rotates an entity's facing
	//Left = 1, Right = 0
	void Rotate(bool dir)
	{
		if (actionFrame > 0)
			return;
		currMaxFrame = 10;
		//currMaxFrame = turnRight.actionFrames.size();

		int newFacing = currentFacing;
		//Turn Right
		if (!dir)
		{
			//WriteDebug("Rotated right" + std::to_string(ID));
			if (currentFacing == West)
				newFacing = 1;
			else
				newFacing += 1;
			//IF PLAYER, ROTATE CAMERA
			if (ID == PLAYER_ENTITYID) 
			{
				Camera* cam = FindCamera(17);
				//cam->PlayCameraAction(turnRight);
			}
		}
		//Turn Left
		else
		{
			//WriteDebug("Rotated left" + std::to_string(ID));

			if (currentFacing == North)
				newFacing = 4;
			else
				newFacing -= 1;
			if (ID == PLAYER_ENTITYID)
			{
				Camera* cam = FindCamera(17);
				//cam->PlayCameraAction(turnLeft);
			}
		}
		ChangeFacing((MapDirection)newFacing);
	}
	
	//Flips an entity's facing by 180 degrees.
	void Flip()
	{
		if (actionFrame > 0)
			return;
		currMaxFrame = 10;
		//currMaxFrame = walkForward.actionFrames.size();

		WriteDebug("Flipped " + std::to_string(ID));
		int currentDir = currentFacing;
		if (currentDir >= 3) {
			currentDir -= 4;
		}
		currentDir += 2;
		if (ID == PLAYER_ENTITYID)
		{
			Camera* cam = FindCamera(17);
			//cam->PlayCameraAction(turn180);
		}
		ChangeFacing((MapDirection)currentDir);
	}

	//Sets an entity's facing without rotating or editing the camera.
	void ChangeFacing(MapDirection dir)
	{
		currentFacing = dir;
	}

	//Moves an entity based on the tile grid. Is independent of facing.
	void Walk(MapDirection dir)
	{
		if (actionFrame > 0)
			return;
		currMaxFrame = 10;
		//currMaxFrame = walkForward.actionFrames.size();

		MapDirection moveDir = dir;
		if (TileMovable(GetCurrentPos(), moveDir))
		{
			if (ID == PLAYER_ENTITYID) 
			{
				Camera* cam = FindCamera(17);
				//cam->PlayCameraAction(walkForward);
			}

			currentMapPos += GetMoveVector(moveDir);
		}
		else
			WriteDebug("Can't Move " + std::to_string(moveDir) + " from tile: " + vecToStr(GetCurrentPos()));
	}

	MapEntity(glm::ivec3 startingPos, MapDirection startingFacing, unsigned int entityID) :
		currentMapPos(startingPos), currentFacing(startingFacing), ID(entityID) {};
};

MapDataChunk& LoadMapData(std::string fileName);

void UpdateMap();
void PlayerStartup(glm::ivec3 startingPos, MapDirection dir);
MapEntity* GetMapEntity(unsigned int entityID);