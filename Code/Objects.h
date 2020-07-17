#pragma once
#include "Physics.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "assets.h"
#include "Components.h"
#include "IDControl.h"

#define PI 3.14159265

const glm::vec3 baseCameraUp = glm::vec3(0.0, 1.0, 0.0);
const glm::vec3 baseCameraFront = glm::vec3(0.0, 0.0, 1.0);


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
			//WriteDebug("RotationVector x angle outside euler angle bounds of -360 <-> 360. X Value set to 0.");
			eulerRotation.x = 0;
		}
		if (eulerRotation.y > INT360 || eulerRotation.y < -INT360)
		{
			//WriteDebug("RotationVector y angle outside euler angle bounds of -360 <-> 360. Y Value set to 0.");
			eulerRotation.y = 0;
		}
		if (eulerRotation.z > INT360 || eulerRotation.z < -INT360)
		{
			//WriteDebug("RotationVector z angle outside euler angle bounds of -360 <-> 360. Z Value set to 0.");
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
		res = RotationVector(newVec, false);
		return res;
	}
	RotationVector operator + (glm::vec3& obj) {
		RotationVector res;
		glm::ivec3 newVec = 
			eulerRotation + glm::ivec3(obj.x * MAX_DECIMAL_PRECISION, obj.y * MAX_DECIMAL_PRECISION, obj.z * MAX_DECIMAL_PRECISION);
		if (newVec.x > INT360) { newVec.x - INT360; }
		if (newVec.x < -INT360) { newVec.x + INT360; }
		if (newVec.y > INT360) { newVec.y - INT360; }
		if (newVec.y < -INT360) { newVec.y + INT360; }
		if (newVec.z > INT360) { newVec.z - INT360; }
		if (newVec.z < -INT360) { newVec.z + INT360; }
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
	RotationVector rotation;
public:

	//PhysicsTransform(glm::ivec3 pos, glm::ivec3 rot) : position(pos), rotation(rot) {};
	PhysicsTransform(glm::vec3 pos, glm::vec3 rot, bool isFloat)
	{
		if (!isFloat)
		{
			position = pos;
			rotation = RotationVector(rot, false);
		}
		else
		{
			position = ConvertFloatVec(pos);
			rotation = RotationVector(ConvertFloatVec(rot), false);
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
		worldRotation.x = (float)rotation.GetEulerRotation().x / MAX_DECIMAL_PRECISION;
		worldRotation.y = (float)rotation.GetEulerRotation().y / MAX_DECIMAL_PRECISION;
		worldRotation.z = (float)rotation.GetEulerRotation().z / MAX_DECIMAL_PRECISION;
		//WriteDebug(vecToStr(worldPosition));
		return  worldRotation;
	}

	void ModifyTransformPos(glm::vec3 newPos) 
	{
		position = ConvertFloatVec(newPos);
	}
	void ModifyTransformRot(glm::vec3 newRot)
	{
		rotation = RotationVector(ConvertFloatVec(newRot), false);
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
	class AnimatorComponent : public ObjectComponent
	{
	private:
		AnimData currentAnim;
		ViewModel* animatee;
		unsigned int currentFrame = 0;
		bool paused = true;
		bool looping = false;
	public:
		AnimatorComponent() {};
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

	class PhysicsComponent: public ObjectComponent 
	{
	private: 
		PhysicsTransform* objectTransform;
		void ComponentUpdate()
		{

		}
	};

	class CameraComponent : public ObjectComponent
	{
		enum CameraType
		{
			Perspective,
			Orthographic,
		};
		enum CameraMode
		{
			LockedOnView,
			FreeView
		};

		//Camera Object. Multiple can be used simultaneously
		class Camera
		{
		private:
			glm::vec3 cameraPos;
			RotationVector rotVec = RotationVector(glm::ivec3(0), false);
			glm::vec3 rotation = glm::vec3(0, 0, 0);
			glm::mat4 cameraView = glm::mat4(1.0f);
			unsigned int currActionFrame;
			//CameraAction currAction = noAction;

		public:

			MAAD_IDController::MAAD_ID* cameraID;
			CameraMode mode = FreeView;
			CameraType cameraType;
			glm::vec3 GetPos() { return cameraPos; }
			glm::vec3 GetCurrentRotation() { return rotation; };
			float cameraFov = 45.0f;

			Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov, CameraType type, int ID = -1) :
				cameraID(GetIDController("Cameras")->CreateNewID(ID)), cameraFov(fov), cameraType(type)
			{
				cameraPos = pos,
					UpdateCameraView();
			};
			Camera() {};

			glm::mat4 GetCameraView() { return cameraView; }

			void RotateCamera(glm::vec3 newRot)
			{
				rotVec = rotVec + newRot;
				UpdateCameraView();
			}

			glm::vec3 CalculateCamFront()
			{
				glm::vec3 finalFront = baseCameraFront;
				glm::vec3 rotation = rotVec.GetEulerRotation();
				//X Axis Operation
				finalFront.x = glm::sin(glm::radians(rotation.x));
				finalFront.z = glm::cos(glm::radians(rotation.x));

				if (rotation.y != 0)
				{
					//Y Axis Operation
					finalFront.x *= glm::cos(glm::radians(rotation.y));
					finalFront.z *= glm::cos(glm::radians(rotation.y));
					finalFront.y = glm::sin(glm::radians(rotation.y));
				}

				//WriteDebug("CameraFront: " + vecToStr(finalFront) + ". Rotation: " + vecToStr(rotation));
				//finalFront = baseCameraFront;
				//WriteDebug("BaseFront: " + vecToStr(finalFront));

				return finalFront;
			};
			glm::vec3 CalculateCamUp()
			{
				glm::vec3 camFront = CalculateCamFront();
				glm::vec3 finalUp = baseCameraUp;
				glm::vec3 rotation = rotVec.GetEulerRotation();

				//Y Axis Operation
				//finalUp.x *= glm::cos(glm::radians(rotation.y +90));
				finalUp.z = glm::cos(glm::radians(rotation.y + 90));
				finalUp.y = glm::sin(glm::radians(rotation.y + 90));

				//finalUp.x = ;
				//finalUp.z = ;

				//finalUp.y = glm::sin(glm::radians(rotation.y + 90));


				//WriteDebug("CameraUp: " + vecToStr(finalUp) + ". Rotation: " + vecToStr(rotation));
				finalUp = baseCameraUp;
				return finalUp;

			};


			//Updates camera view based on its offset and its current position.
			//cameraView is based on cameraFront and cameraUp, not on the rotation of the camera.
			void UpdateCameraView()
			{
				cameraView = glm::lookAt(cameraPos, cameraPos + CalculateCamFront(), CalculateCamUp());;
			}
			//Teleports the camera to a new location. Does not reset the camera offset.
			//--Overloads--
			//CameraCoords()
			//glm::vec3
			void MoveCamera(glm::vec3 newPos)
			{
				cameraPos = newPos;
				UpdateCameraView();
			}
			//Teleports the camera to a new location. Does not reset the camera offset.
			//--Overloads--
			//CameraCoords()
			//glm::vec3
			//void MoveCamera(glm::vec3 moveTo)
			//{
			//	//cameraCoords.cameraPos += moveTo;
			//	UpdateCameraView();
			//}



			//Sets the current CameraAction and sets current active frame to 0
			//void PlayCameraAction(CameraAction action) 
			//{

			//	currAction = action;
			//	currActionFrame = 0;

			//}

			//Rotates camera action coordinates based upon the current rotation of the camera
			//CameraCoords RotateCoords(CameraCoords check) 
			//{
			//	CameraCoords newCoords = check;
			//	//WriteDebug("CurrRotation: " + vecToStr(currRotationAngle) + " Frame Rotation: " + vecToStr(newCoords.rotation));
			//	glm::quat checkQuat = glm::radians((currRotationAngle + newCoords.rotation) * 0.5f);
			//	//WriteDebug("Quat: " + quatToStr(checkQuat));
			//	//WriteDebug("Inverse: " + quatToStr(glm::inverse(checkQuat)));
			//	glm::vec3 rotatedPoint = checkQuat * newCoords.cameraPos * glm::inverse(checkQuat);
			//	glm::vec3 rotatedFront = checkQuat * newCoords.cameraFront * glm::inverse(checkQuat);
			//	glm::vec3 rotatedUp = checkQuat * newCoords.cameraUp * glm::inverse(checkQuat);

			//	//WriteDebug("Rotated: " + vecToStr(rotatedFront));
			//	CameraCoords rotatedCoords = CameraCoords(rotatedPoint, newCoords.rotation, rotatedFront, rotatedUp);

			//	return rotatedCoords;
			//}

			//Updates the current frame of the camera and the camera view. Runs once every RenderUpdate().
			void UpdateCamera()
			{
				UpdateCameraView();

				//if (currAction.ID != noAction.ID) 
				//{
				//	if (currActionFrame < currAction.actionFrames.size())
				//	{
				//		offsetCoords = RotateCoords(currAction.actionFrames[currActionFrame]);
				//		currActionFrame++;
				//	}
				//	else 
				//	{
				//		if (currAction.ID != noAction.ID) 
				//		{
				//			cameraCoords = cameraCoords + offsetCoords;
				//			currRotationAngle += currAction.actionFrames[currAction.actionFrames.size() - 1].rotation;
				//			//WriteDebug(vecToStr(cameraCoords.cameraFront));
				//			offsetCoords = CameraCoords();
				//			currAction = noAction;
				//		}
				//	}
				//}
			}
		};

	};


private:
	std::vector<ObjectComponent> components;


public:
	void AddComponent()
	{
		
	}
};

void ObjectUpdate();