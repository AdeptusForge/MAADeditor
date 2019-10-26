#include "framework.h"
#include "physics.h"
#include "Assets.h"
#include "Objects.h"
#include "Debug.h"
#include "FileControl.h"

std::vector<MAADPhysicsObject> allPhysObjects;

glm::ivec3 ConvertFloatVector(glm::vec3 floatVec) 
{
	glm::ivec3 newVec;
	newVec.x = floatVec.x * MAX_DECIMAL_PRECISION;
	newVec.y = floatVec.y * MAX_DECIMAL_PRECISION;
	newVec.z = floatVec.z * MAX_DECIMAL_PRECISION;
	return newVec;
}


void CreatePhysicsObject(glm::vec3 position, glm::vec3 rotation, PhysicsLock lock, glm::vec3 velocity, glm::vec3 spin) 
{
	allPhysObjects.push_back(MAADPhysicsObject(PhysicsLocation(position, rotation), lock, velocity, spin));
}

void PhysicsUpdate() 
{
	for (int i = 0; i < allPhysObjects.size(); i++) 
	{
		allPhysObjects[i].UpdateObject();
	}
}