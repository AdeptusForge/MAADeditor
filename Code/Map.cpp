#include "Map.h"

MapTile getTile(MapDataChunk map, unsigned int x, unsigned int y)
{
	return map.tileMap.at(y * map.mapSize[1] + x);
}

TileFeature getTileFeature(MapTile tile, unsigned int direction)
{
	return (TileFeature)tile.features[direction];
}
Map::Map(std::string filename)
{
	map = LoadMapData(filename);
	//note we need to specify start position/rotation for the player when we load a map for now im just setting it to 0's
	playerX = 0;
	playerY = 0;
	playerDirection = (MapDirection)0;
}

glm::ivec2 Map::Move(MapDirection direction)
{
	MapTile current = getTile(map, playerX, playerY);
	unsigned int dirTravel = playerDirection + direction;
	if (dirTravel > 3) {
		dirTravel -= 4;
	}
	int moveX = playerX += glm::sin(glm::pi<float>() * direction / 2);
	int moveY = playerY += glm::cos(glm::pi<float>() * direction / 2);
	TileFeature currentFeature = getTileFeature(current, dirTravel);
	
	switch (currentFeature) { //for later use implementing doors and such
		case 0:
			playerX += moveX;
			playerY = moveY;
			return glm::ivec2(moveX, moveY); //return camera movement
			break;
		case 1:
			return glm::ivec2(10 + moveX, 10 + moveY); //indicate direction camera will "bump"
			break;
	}
}

void Map::Rotate(int direction) {
	int currentDirection = playerDirection;
	if (currentDirection += direction < 0) {
		playerDirection = (MapDirection)3;
	}
	else if (currentDirection += direction > 4) {
		playerDirection = (MapDirection)0;
	}
	else {
		playerDirection = (MapDirection)(currentDirection += direction);
	}
}

void Map::TurnAround() {
	int currentDirection = playerDirection +2;
	if (currentDirection > 3){
		currentDirection -= 4;
	}
	playerDirection = (MapDirection)currentDirection;

}





