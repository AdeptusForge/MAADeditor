#include "framework.h"
#include "physics.h"
#include "Assets.h"
#include "Objects.h"
#include "Debug.h"
#include "FileControl.h"

std::vector<MAADPhysicsObject> allPhysObjects;

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