#pragma once

#include "framework.h"
#include "Debug.h"
#include "algorithm"
#include "SDL.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "SDL_mixer.h"


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
	Wall = 1

};