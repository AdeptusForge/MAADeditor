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
#include "bounding.h"

void PhysicsStartup();

//REFACTOR::REWORK ALL OF THE PHYSICS SYSTEM into the object system.
void PhysicsUpdate();



//class MAAD_PhysicsObject
//{
//public:
//	unsigned int ID = 0;
//	PhysicsTransform trans;
//
//	MAAD_PhysicsObject(PhysicsTransform transform, glm::vec3 vel = glm::vec3(), glm::vec3 spin = glm::vec3())
//	{
//		intVelocity = ConvertFloatVec(vel);
//		intSpin = ConvertFloatVec(spin);
//	};
//	MAAD_PhysicsObject() {};
//	
//	void UpdateObject() 
//	{
//		WriteDebug(vecToStr(trans.GetWorldPosition()));
//	}
//
//private:
//	glm::ivec3 intVelocity; //change in position per frame (not accounting physics interactions)
//	glm::ivec3 intSpin; //change in position per frame (not accounting physics interactions)
//};

//void CreatePhysicsObject(PhysicsTransform transform);