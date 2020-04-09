#pragma once
#include "FileControl.h"
#include "framework.h"
#include "Debug.h"
#include "algorithm"
#include "SDL.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "SDL_mixer.h"




enum MapDirection 
{
	North = 0,
	East = 1,
	South = 2,
	West = 3
};

enum TileFeature 
{
	Empty = 0,
	Wall = 1,
};

MapTile getTile(MapDataChunk map, unsigned int x, unsigned int y);

TileFeature getTileFeature(MapTile tile, unsigned int direction);

class Map
{
private:
	unsigned int playerX;
	unsigned int playerY;
	MapDirection playerDirection;
	MapDataChunk map;

public:
	Map(std::string filename);
	~Map();
	glm::ivec2 Move(MapDirection direction);
	void Rotate(int direction); //negative or positive
	void TurnAround();


};

