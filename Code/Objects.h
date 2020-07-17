#pragma once
#include "Physics.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "assets.h"
#include "Components.h"
#include "IDControl.h"

const unsigned int MAX_DECIMAL_PRECISION = 10000;
const int INT360 = 3600000;
glm::ivec3 ConvertFloatVec(glm::vec3 floatVec);

struct RotationVector 
{
private:
	glm::ivec3 eulerRotation;
public:
	RotationVector() {};

	RotationVector(glm::vec3 rot, bool isFloat)
	{
		if (isFloat)
			eulerRotation = ConvertFloatVec(rot);
		else
			eulerRotation = rot;

		if (eulerRotation.x > INT360 || eulerRotation.x < -INT360)
		{
			WriteDebug("RotationVector x angle outside euler angle bounds of -360 <-> 360. X Value set to 0.");
			eulerRotation.x = 0;
		}
		if (eulerRotation.y > INT360 || eulerRotation.y < -INT360)
		{
			WriteDebug("RotationVector y angle outside euler angle bounds of -360 <-> 360. Y Value set to 0.");
			eulerRotation.y = 0;
		}
		if (eulerRotation.z > INT360 || eulerRotation.z < -INT360)
		{
			WriteDebug("RotationVector z angle outside euler angle bounds of -360 <-> 360. Z Value set to 0.");
			eulerRotation.z = 0;
		}
	}

	glm::ivec3 GetTrueRotation() { return eulerRotation; }
	glm::vec3 GetEulerRotation()
	{
		glm::vec3 rot;
		//WriteDebug(vecToStr(position));
		rot.x = (float)eulerRotation.x / MAX_DECIMAL_PRECISION;
		rot.y = (float)eulerRotation.y / MAX_DECIMAL_PRECISION;
		rot.z = (float)eulerRotation.z / MAX_DECIMAL_PRECISION;
		//WriteDebug(vecToStr(worldPosition));
		return  rot;
	}
	RotationVector operator + (RotationVector & obj) {
		RotationVector res;
		glm::ivec3 newVec = eulerRotation + obj.GetTrueRotation();
		if (newVec.x > INT360) { newVec.x - INT360; }
		if (newVec.x < -INT360) { newVec.x + INT360; }
		if (newVec.y > INT360) { newVec.y - INT360; }
		if (newVec.y < -INT360) { newVec.y + INT360; }
		if (newVec.z > INT360) { newVec.z - INT360; }
		if (newVec.z < -INT360) { newVec.z + INT360; }
		WriteDebug(vecToStr(newVec));
		res = RotationVector(newVec, false);
		return res;
	}
	RotationVector operator - (RotationVector& obj) {
		RotationVector res;
		glm::ivec3 newVec = eulerRotation - obj.GetTrueRotation();
		if (newVec.x > INT360) { newVec.x - INT360; }
		if (newVec.x < -INT360) { newVec.x + INT360; }
		if (newVec.y > INT360) { newVec.y - INT360; }
		if (newVec.y < -INT360) { newVec.y + INT360; }
		if (newVec.z > INT360) { newVec.z - INT360; }
		if (newVec.z < -INT360) { newVec.z + INT360; }
		WriteDebug(vecToStr(newVec));
		res = RotationVector(newVec, false);
		return res;
	}
};


struct PhysicsTransform
{
private:
	glm::ivec3 position;
	glm::ivec3 rotation;
public:

	//PhysicsTransform(glm::ivec3 pos, glm::ivec3 rot) : position(pos), rotation(rot) {};
	PhysicsTransform(glm::vec3 pos, glm::vec3 rot, bool isFloat)
	{
		if (!isFloat)
		{
			position = pos;
			rotation = rot;
		}
		else
		{
			position = ConvertFloatVec(pos);
			rotation = ConvertFloatVec(rot);
		}
	}
	PhysicsTransform() {};


	glm::vec3 GetWorldPosition()
	{
		glm::vec3 worldPosition;
		//WriteDebug(vecToStr(position));
		worldPosition.x = (float)position.x / MAX_DECIMAL_PRECISION;
		worldPosition.y = (float)position.y / MAX_DECIMAL_PRECISION;
		worldPosition.z = (float)position.z / MAX_DECIMAL_PRECISION;
		//WriteDebug(vecToStr(worldPosition));
		return  worldPosition;
	}
	glm::vec3 GetWorldRotation()
	{
		glm::vec3 worldRotation;
		//WriteDebug(vecToStr(position));
		worldRotation.x = (float)rotation.x / MAX_DECIMAL_PRECISION;
		worldRotation.y = (float)rotation.y / MAX_DECIMAL_PRECISION;
		worldRotation.z = (float)rotation.z / MAX_DECIMAL_PRECISION;
		//WriteDebug(vecToStr(worldPosition));
		return  worldRotation;
	}


	void ModifyTransformPos(glm::vec3 newPos) 
	{
		position = ConvertFloatVec(newPos);
	}
	void ModifyTransformRot(glm::vec3 newRot)
	{
		rotation = ConvertFloatVec(newRot);
	}

	//Original Physics Lock conserved. NOT modified.
	PhysicsTransform operator + (PhysicsTransform const& obj) {
		PhysicsTransform res;
		res.position = position + obj.position;
		return res;
	}
};

class ViewModel
{
public:
	unsigned int ID;
	PhysicsTransform modelOffset;
	Model viewModel;
private:

};

class MAAD_GameObject
{
private:
	PhysicsTransform transform;
	std::string name;
	MAAD_IDController::MAAD_ID* objectID;
	std::vector<ViewModel> models;
public:
	MAAD_GameObject(std::string newName, int newID = -1, PhysicsTransform trans = PhysicsTransform()) :
		name(newName), transform(trans), objectID(GetIDController("GameObjects")->CreateNewID(newID)) {};
	MAAD_GameObject() {};
	
	std::vector<ViewModel> GetModels() { return models; }
	PhysicsTransform GetTransform() { return transform; }
	void MoveObjectToPosition(glm::vec3 vec) 
	{
		transform.ModifyTransformPos(vec);
	}
	void MoveObjectRelative(glm::vec3 vec)
	{
		transform.ModifyTransformPos(transform.GetWorldPosition() + vec);
	}

	class ObjectComponent
	{
	private:
		MAAD_GameObject* parent;
	protected:
		virtual void ComponentUpdate()
		{
		}
	public:
		ObjectComponent() {};

	};
	class Animator : ObjectComponent
	{
	private:
		AnimData currentAnim;
		ViewModel* animatee;
		unsigned int currentFrame = 0;
		bool paused = true;
		bool looping = false;
	public:
		Animator() {};
		void ChangeAnimatee(ViewModel* newModel)
		{
			currentFrame = 0;
			if (animatee != newModel)
				animatee = newModel;
		}

		void ComponentUpdate()
		{
			if (!paused) { currentFrame++; }


			//AnimEvent Check & Activation.
			//if(currentAnim.GetCurrFrame(currentFrame))








			if (currentFrame < currentAnim.GetLength())
			{
				if (!looping)
				{
					paused = true;
				}
				currentFrame = 0;
			}

		}

		//Plays the current animation where it last left off.
		void Play()
		{
			paused = false;
		}
		//Overload that changes the animation in question.
		void Play(AnimData newAnim)
		{
			currentFrame = 0;
		}

		void Pause()
		{
			paused = true;
		}
	};

	class PhysicsComponent: ObjectComponent 
	{
	private: 
		PhysicsTransform* objectTransform;
		void ComponentUpdate()
		{

		}
	};


	void AddComponent()
	{

	}
private:
	std::vector<ObjectComponent> components;
};

void ObjectUpdate();