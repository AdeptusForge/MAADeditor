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

void PhysicsUpdate();

const unsigned int MAX_DECIMAL_PRECISION = 10000;


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
		position = glm::vec3((float)pos.x * MAX_DECIMAL_PRECISION,
			(float)pos.y *MAX_DECIMAL_PRECISION,
			(float)pos.z * MAX_DECIMAL_PRECISION);
		rotation = glm::vec3((float)rot.x * MAX_DECIMAL_PRECISION,
			(float)rot.y * MAX_DECIMAL_PRECISION,
			(float)rot.z * MAX_DECIMAL_PRECISION);
	}
	PhysicsLocation() {};

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
		intVelocity = glm::vec3((float)vel.x * MAX_DECIMAL_PRECISION,
			(float)vel.y * MAX_DECIMAL_PRECISION,
			(float)vel.z * MAX_DECIMAL_PRECISION);
		intSpin = glm::vec3((float)spin.x * MAX_DECIMAL_PRECISION,
			(float)spin.y * MAX_DECIMAL_PRECISION,
			(float)spin.z * MAX_DECIMAL_PRECISION);
	};
	MAADPhysicsObject() {};
	
	void UpdateObject() 
	{
		PhysicsLocation newLoc = PhysicsLocation(glm::ivec3(currLocation.position + intVelocity), glm::ivec3(currLocation.rotation + intSpin));
		currLocation = newLoc;
	}

private:
	glm::ivec3 intVelocity; //change in position per frame (not accounting physics interactions)
	glm::ivec3 intSpin; //change in position per frame (not accounting physics interactions)
};
