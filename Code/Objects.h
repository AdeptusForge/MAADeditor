#pragma once
#include "Physics.h"
#include <vector>

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
	PhysicsObject obj;
	std::string name;
	std::vector <GameComponent> components;

	Point3 GetPosition() { return obj.pos.point; };

	Rotation GetRotation() { return obj.pos.rotation; };
};


