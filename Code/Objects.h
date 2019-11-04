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


public:
	PhysicsTransform transform;
	std::string name;
	int ID;
	std::vector<ViewModel> models;
};


