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


//REFACTOR::REWORK ALL OF THE PHYSICS SYSTEM, PREFERABLY WITH FIXED DECIMAL PRECISION.
void PhysicsUpdate();

const unsigned int MAX_DECIMAL_PRECISION = 10000;

glm::ivec3 ConvertFloatVec(glm::vec3);

struct PhysicsTransform
{
private:
	glm::ivec3 position;
	glm::ivec3 rotation;
public:

	PhysicsTransform(glm::ivec3 pos, glm::ivec3 rot) : position(pos), rotation(rot) {};
	PhysicsTransform() {};

	glm::vec3 GetWorldPosition() 
	{
		glm::vec3 worldPosition;
		worldPosition.x = (float)position.x / MAX_DECIMAL_PRECISION;
		worldPosition.y = (float)position.y / MAX_DECIMAL_PRECISION;
		worldPosition.z = (float)position.z / MAX_DECIMAL_PRECISION;
		//WriteDebug(vecToStr(worldPosition));
		return  worldPosition;
	}


	//Original Physics Lock conserved. NOT modified.
	PhysicsTransform operator + (PhysicsTransform const& obj) {
		PhysicsTransform res;
		res.position = position + obj.position;
		return res;
	}
};

class MAADPhysicsObject
{
public:
	unsigned int ID = 0;
	
	MAADPhysicsObject(PhysicsTransform transform, glm::vec3 vel, glm::vec3 spin)
	{
		intVelocity = ConvertFloatVec(vel);
		intSpin = ConvertFloatVec(spin);
	};
	MAADPhysicsObject() {};
	
	void UpdateObject() 
	{

	}

private:
	glm::ivec3 intVelocity; //change in position per frame (not accounting physics interactions)
	glm::ivec3 intSpin; //change in position per frame (not accounting physics interactions)
};
