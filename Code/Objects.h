#pragma once
#include "Physics.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "assets.h"

class GameComponent
{
public:
	virtual void ComponentFunction() = 0;
private:

};

class GameObject
{
private:
public:
	MAADPhysicsObject obj;


	//MODEL MOVE TEST ONLY
	Model objModel;
	std::string name;
	std::vector <GameComponent> components;
};


