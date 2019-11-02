#pragma once
#include "Physics.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "assets.h"
#include "Components.h"

class GameObject
{
private:
	PhysicsTransform transform;

	void Initialize() 
	{
		UpdateComponentData();
	}
	void UpdateComponentData()
	{
		for (int i = 0; i < components.size(); i++) 
		{
			components[i].objectData = ComponentDataChunk(&transform);
		}
	}
public:
	std::string name;
	int ID;
	std::vector <GameComponent> components;
};


