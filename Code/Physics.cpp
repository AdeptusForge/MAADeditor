#include "framework.h"
#include "physics.h"
#include "Assets.h"
#include "Objects.h"
#include "Debug.h"
#include "FileControl.h"

//std::vector<MAAD_PhysicsObject> allPhysObjects;



//void CreatePhysicsObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 velocity, glm::vec3 spin) 
//{
//	allPhysObjects.push_back(MAADPhysicsObject(PhysicsTransform(position, rotation), lock, velocity, spin));
//}

//void PhysicsStartup()
//{
//	CreatePhysicsObject(PhysicsTransform());
//}


//void PhysicsUpdate() 
//{
//	for (int i = 0; i < allPhysObjects.size(); i++)
//	{
//		allPhysObjects[i].UpdateObject();
//	}
//}

//void CreatePhysicsObject(PhysicsTransform transform)
//{
//	allPhysObjects.push_back(MAAD_PhysicsObject(transform));
//}