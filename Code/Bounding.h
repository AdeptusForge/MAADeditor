#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Debug.h"
#include <Math.h>


class MAAD_BoundingBox 
{
private:
	typedef void (*BB_OverlapCallbackFunc)();
	//Used when two MAAD_BoundingBoxes overlap
	BB_OverlapCallbackFunc overlapCallbackFunc;
protected:
	glm::vec3 dim;
	glm::vec3 pos;

public:
	void SetDimensions(glm::vec3 newDimensions) { dim = newDimensions; };
	void SetPosition(glm::vec3 newPosition) { pos = newPosition; };
	MAAD_BoundingBox(glm::vec3 dimensions = glm::vec3(0), glm::vec3 position = glm::vec3(0),
		BB_OverlapCallbackFunc overlapCallback = nullptr) :
	dim(dimensions), pos(position), overlapCallbackFunc(overlapCallback){};
	MAAD_BoundingBox() {};

};
