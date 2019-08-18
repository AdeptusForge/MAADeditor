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
	string name;
	vector <GameComponent> components;

	Point3 GetPosition() { return obj.pos.point; };

	Rotation GetRotation() { return obj.pos.rotation; };
};


