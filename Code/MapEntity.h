#pragma once

#include "framework.h"
#include "Debug.h"
#include "algorithm"
#include "SDL.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Map.h"

class MapEntity 
{
private:
	glm::ivec2 currentMapPos;
	MapDirection currentFacing;
public:
	glm::ivec2 GetCurrentPos() { return currentMapPos; }
	//Left = 1, Right = 0
	void Rotate(bool dir) 
	{
		int newFacing = currentFacing;
		//Turn Right
		if(!dir)
		{
			if (currentFacing == West)
				newFacing = 1;
			else
				newFacing += 1;


			//IF PLAYER, ROTATE CAMERA
		}
		//Turn Left
		else 
		{
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
		int currentDir = currentFacing + 2;
		if (currentDir > 3) {
			currentDir -= 4;
		}


		//IF PLAYER, ROTATE CAMERA
		ChangeFacing((MapDirection)currentDir);
	}
	void ChangeFacing(MapDirection dir) 
	{
		currentFacing = dir;
	}
	void Move(MapDirection dir) 
	{
		if (TileMovable(GetCurrentPos(), dir)) 
		{
			WriteDebug("Can Move " +std::to_string(dir) + "from tile: " 
				+ std::to_string(GetCurrentPos().x) +", " + std::to_string(GetCurrentPos().y));
		}
		//IF PLAYER, MOVE CAMERA
	}
	MapEntity() 
	{ currentMapPos = glm::ivec2(1, 0);
	currentFacing = North; 
	}
};

