#pragma once
#include <string>
#include <iostream> 
#include <iterator> 
#include "framework.h"
#include "Assets.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Debug.h"
#include <Math.h>

void PhysicsUpdate();

const unsigned int MAX_DECIMAL_PRECISION = 10000;

glm::ivec3 ConvertFloatVector(glm::vec3);

struct PhysicsTransform 
{
public:
	glm::ivec3 position;
	glm::ivec3 rotation;
	bool lockedXmov = false;
	bool lockedYmov = false;
	bool lockedZmov = false;
	bool lockedXspin = false;
	bool lockedYspin = false;
	bool lockedZspin = false;
	PhysicsTransform(glm::ivec3 pos, glm::ivec3 rot) : position(pos), rotation(rot) {};
	PhysicsTransform(glm::vec3 pos, glm::vec3 rot) 
	{
		position = ConvertFloatVector(pos);
		rotation = ConvertFloatVector(rot);
	}
	PhysicsTransform() {};

	glm::vec3 GetWorldPosition() 
	{
		glm::vec3 worldPosition;
		worldPosition.x = (float)position.x / MAX_DECIMAL_PRECISION;
		worldPosition.y = (float)position.y / MAX_DECIMAL_PRECISION;
		worldPosition.z = (float)position.z / MAX_DECIMAL_PRECISION;
		return  worldPosition;
	}
	glm::vec3 GetWorldRotation()
	{
		glm::vec3 worldRotation;
		worldRotation.x = (float)rotation.x / MAX_DECIMAL_PRECISION;
		worldRotation.y = (float)rotation.y / MAX_DECIMAL_PRECISION;
		worldRotation.z = (float)rotation.z / MAX_DECIMAL_PRECISION;
		return  worldRotation;
	}
};

class MAADPhysicsObject
{
public:
	unsigned int ID = 0;
	
	MAADPhysicsObject(PhysicsTransform transform, glm::vec3 vel, glm::vec3 spin)
	{
		intVelocity = ConvertFloatVector(vel);
		intSpin = ConvertFloatVector(spin);
	};
	MAADPhysicsObject() {};
	
	void UpdateObject() 
	{

	}

private:
	glm::ivec3 intVelocity; //change in position per frame (not accounting physics interactions)
	glm::ivec3 intSpin; //change in position per frame (not accounting physics interactions)
};
