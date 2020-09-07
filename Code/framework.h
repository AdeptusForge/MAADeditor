// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

enum MapDirection
{
	North = 1,
	East = 2,
	South = 3,
	West = 4,
	Up = 5,
	Down = 6
};
enum DamageType 
{
	Untyped,
	Edged,
	Blunt,
	Malphorm
};