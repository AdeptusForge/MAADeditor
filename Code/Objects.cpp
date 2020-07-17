#include "Objects.h"

std::vector<MAAD_GameObject> allGameObjects;

glm::ivec3 ConvertFloatVec(glm::vec3 floatVec)
{
	glm::ivec3 newVec;
	newVec.x = floatVec.x * MAX_DECIMAL_PRECISION;
	newVec.y = floatVec.y * MAX_DECIMAL_PRECISION;
	newVec.z = floatVec.z * MAX_DECIMAL_PRECISION;
	return newVec;
}


void ObjectUpdate() 
{

};