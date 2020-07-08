#pragma once

#include "framework.h"
#include <fstream>
#include <String>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "debug.h"
#include <vector>



enum IDTYPE
{
	RENDER_ID,
	INVENTORY_ID,
	CAMERA_ID,
};

unsigned int GetFirstAvailableID(IDTYPE type);
void IDStartup();

class MAAD_ID
{
private:
	IDTYPE type;
	unsigned int id;
	
public:
	unsigned int ID() { return id; }
	IDTYPE Type() { return type; }

	MAAD_ID(IDTYPE newtype)
	{
		type = newtype;
		id = GetFirstAvailableID(newtype);
	}
};
void RecycleID(MAAD_ID id);
