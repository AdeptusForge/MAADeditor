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

struct PhysicsLock
{
	bool lockedXmov = false;
	bool lockedYmov = false;
	bool lockedZmov = false;
	bool lockedXspin = false;
	bool lockedYspin = false;
	bool lockedZspin = false;

	PhysicsLock(bool lxM, bool lyM, bool lzM, bool lxS, bool lyS, bool lzS) :
		lockedXmov(lxM), lockedYmov(lyM), lockedZmov(lzM), lockedXspin(lxS), lockedYspin(lyS), lockedZspin(lzS) {};
	PhysicsLock() {};
};

void CreatePhysicsObject(glm::vec3, glm::vec3, PhysicsLock, glm::vec3, glm::vec3);

struct PhysicsLocation 
{
public:
	glm::ivec3 position;
	glm::ivec3 rotation;
	PhysicsLocation(glm::ivec3 pos, glm::ivec3 rot) : position(pos), rotation(rot) {};
	PhysicsLocation(glm::vec3 pos, glm::vec3 rot) 
	{
		position = ConvertFloatVector(pos);
		rotation = ConvertFloatVector(rot);
	}
	PhysicsLocation() {};

	glm::vec3 GetWorldPosition() 
	{
		glm::vec3 worldPosition;
		worldPosition.x = position.x / MAX_DECIMAL_PRECISION;
		worldPosition.y = position.y / MAX_DECIMAL_PRECISION;
		worldPosition.z = position.z / MAX_DECIMAL_PRECISION;
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
	PhysicsLocation currLocation;
	PhysicsLock lock;


	PhysicsLocation EditPosition(glm::vec3 newPos, glm::vec3 newRot)
	{
		
		if (lock.lockedXmov == false)
			currLocation.position.x = newPos.x;
		if (lock.lockedYmov == false)
			currLocation.position.y = newPos.y;
		if (lock.lockedZmov == false)
			currLocation.position.z = newPos.z;
		if (lock.lockedXspin == false)
			currLocation.rotation.x = newRot.x;
		if (lock.lockedYspin == false)
			currLocation.rotation.y = newRot.y;
		if (lock.lockedZspin == false)
			currLocation.rotation.z = newRot.z;
	}
	//Overload
	PhysicsLocation EditPosition(PhysicsLocation loc)
	{
		if (lock.lockedXmov == false)
			currLocation.position.x = loc.position.x;
		if (lock.lockedYmov == false)
			currLocation.position.y = loc.position.y;
		if (lock.lockedZmov == false)
			currLocation.position.z = loc.position.z;
		if (lock.lockedXspin == false)
			currLocation.rotation.x = loc.rotation.x;
		if (lock.lockedYspin == false)
			currLocation.rotation.y = loc.rotation.y;
		if (lock.lockedZspin == false)
			currLocation.rotation.z = loc.rotation.z;
	}

	MAADPhysicsObject(PhysicsLocation loc, PhysicsLock locker, glm::vec3 vel, glm::vec3 spin) : currLocation(loc), lock(locker)
	{
		intVelocity = ConvertFloatVector(vel);
		intSpin = ConvertFloatVector(spin);
	};
	MAADPhysicsObject() {};
	
	void UpdateObject() 
	{

		PhysicsLocation newLoc = PhysicsLocation(currLocation.position + intVelocity, currLocation.rotation + intSpin);
		currLocation = newLoc;
	}

private:
	glm::ivec3 intVelocity; //change in position per frame (not accounting physics interactions)
	glm::ivec3 intSpin; //change in position per frame (not accounting physics interactions)
};
