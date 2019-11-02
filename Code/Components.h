#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "assets.h"
#include "Physics.h"

struct ComponentDataChunk 
{
	PhysicsTransform *objectTransform;

	ComponentDataChunk(PhysicsTransform* objTransform) : objectTransform(objTransform) {};
};


class GameComponent
{
public:
	ComponentDataChunk objectData;
	virtual void ComponentFunction() = 0;
private:
};

class StandardPhysicsComponent : public GameComponent 
{
	
};

class ViewModelComponent : public GameComponent 
{
	PhysicsTransform modelOffset;
	Model viewModel;
	void ComponentFunction() 
	{

	}
};

class Collider2DComponent : public GameComponent
{

};

class Collider3DComponent : public GameComponent
{

};

class AIControllerComponent : public GameComponent
{

};