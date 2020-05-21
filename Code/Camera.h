#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include "Physics.h"
#define PI 3.14159265

const glm::vec3 baseCameraUp = glm::vec3(0.0,1.0,0.0);
const glm::vec3 baseCameraFront = glm::vec3(0.0, 0.0, 1.0);

//REFACTOR:: Into physics-based coordinate system.
struct CameraCoords
{
	glm::vec3 cameraPos;
	glm::vec3 rotation;
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;
	glm::bvec3 rotationLock;
	CameraCoords(glm::vec3 pos, glm::vec3 rot, glm::vec3 front = baseCameraFront, glm::vec3 up = baseCameraUp, glm::bvec3 lock = glm::bvec3(false)):
		cameraPos(pos), rotation(rot), cameraUp(up), cameraFront(front), rotationLock(lock){};
	CameraCoords() {};

	CameraCoords operator + (const CameraCoords& b) 
	{
		CameraCoords coords;
		coords.cameraPos = this->cameraPos + b.cameraPos;
		coords.rotation = this->rotation + b.rotation;
		coords.cameraFront = b.cameraFront;
		coords.cameraUp = b.cameraUp;
		return coords;
	}
};

//REFACTOR:: Remake this as an animation
struct CameraAction 
{
	unsigned int ID;
	std::vector<CameraCoords> actionFrames;
	CameraAction(unsigned int id, std::vector<CameraCoords> frames) : ID(id), actionFrames(frames) {};
	CameraAction() {};
};

#pragma region Hardcoded CameraActions
const CameraAction noAction = CameraAction(0, std::vector<CameraCoords>{});
const CameraAction walkForward = CameraAction(1, std::vector<CameraCoords> 
{
	CameraCoords(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0,0,0)),
	CameraCoords(glm::vec3(0.0, 0.0, 2.0), glm::vec3(0,0,0)),
	CameraCoords(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0,0,0)),
	CameraCoords(glm::vec3(0.0, 0.0, 4.0), glm::vec3(0,0,0)),
	CameraCoords(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0,0,0)),
	CameraCoords(glm::vec3(0.0, 0.0, 6.0), glm::vec3(0,0,0)),
	CameraCoords(glm::vec3(0.0, 0.0, 7.0), glm::vec3(0,0,0)),
	CameraCoords(glm::vec3(0.0, 0.0, 8.0), glm::vec3(0, 0, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 9.0), glm::vec3(0, 0, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 10.0), glm::vec3(0, 0, 0))

});
const CameraAction turnRight = CameraAction(2, std::vector<CameraCoords>
{
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0.0, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -15, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -30, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -45, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -60, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -75, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -90, 0))
});
const CameraAction turnLeft = CameraAction(3, std::vector<CameraCoords>
{
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 15, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 30, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 45, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 60, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 75, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 90, 0))
});
const CameraAction turn180 = CameraAction(4, std::vector<CameraCoords>
{
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 30, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 60, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 90, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 120, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 150, 0)),
	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 180, 0))
});
//const CameraAction walkBackward;
//const CameraAction walkRight;
//const CameraAction walkLeft;
//const CameraAction turnRight;
//const CameraAction turnLeft;
//const CameraAction turn180;

#pragma endregion

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
//REFACTOR:: Restructure CameraActions and
class Camera 
{
private:
	glm::vec3 currRotationAngle = glm::vec3(0, 0, 0);
	CameraCoords cameraCoords;
	glm::mat4 cameraView = glm::mat4(1.0f);
	unsigned int currActionFrame;
	CameraAction currAction = noAction;

public:
	unsigned int cameraID;
	CameraMode mode = FreeView;
	CameraType cameraType;
	CameraCoords offsetCoords = CameraCoords(glm::vec3(0), glm::vec3(0));
	CameraCoords GetCameraCoords() { return cameraCoords; }
	float cameraFov = 45.0f;

	Camera(unsigned int ID, glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov, CameraType type):
		cameraID(ID), cameraFov(fov), cameraType(type)
	{
		cameraCoords.cameraPos = pos,
		cameraCoords.cameraFront = baseCameraFront;
		cameraCoords.cameraUp = baseCameraUp;
		UpdateCameraView();
	};
	Camera() {};

	glm::mat4 GetCameraView() { return cameraView; }

	//Updates camera view based on its offset and its current position.
	//cameraView is based on cameraFront and cameraUp, not on the rotation of the camera.
	void UpdateCameraView() 
	{
		//if(mode == FreeView)
		CameraCoords offsetAndBase = cameraCoords + offsetCoords;
		//WriteDebug("Forward:: Normal:" + vecToStr(cameraCoords.cameraFront) + ". Offset: " + vecToStr(offsetCoords.cameraFront));
		//WriteDebug("Up:: Normal:" + vecToStr(cameraCoords.cameraUp) + ". Offset: " + vecToStr(offsetCoords.cameraUp));

		cameraView = glm::lookAt(offsetAndBase.cameraPos, offsetAndBase.cameraPos + offsetAndBase.cameraFront, offsetAndBase.cameraUp);
	}
	//Teleports the camera to a new location. Does not reset the camera offset.
	//--Overloads--
	//CameraCoords()
	//glm::vec3
	void MoveCamera(CameraCoords newCoords) 
	{
		cameraCoords.cameraPos = newCoords.cameraPos;
		UpdateCameraView();
	}
	//Teleports the camera to a new location. Does not reset the camera offset.
	//--Overloads--
	//CameraCoords()
	//glm::vec3
	void MoveCamera(glm::vec3 moveTo)
	{
		cameraCoords.cameraPos += moveTo;
		UpdateCameraView();
	}
	//Sets the current CameraAction and sets current active frame to 0
	void StartCameraAction(CameraAction action) 
	{
		currAction = action;
		currActionFrame = 0;

	}
	
	//Rotates camera action coordinates based upon the current rotation of the camera
	CameraCoords RotateCoords(CameraCoords check) 
	{
		CameraCoords newCoords = check;
		//WriteDebug("CurrRotation: " + vecToStr(currRotationAngle) + " Frame Rotation: " + vecToStr(newCoords.rotation));
		glm::quat checkQuat = glm::radians((currRotationAngle + newCoords.rotation) * 0.5f);
		//WriteDebug("Quat: " + quatToStr(checkQuat));
		//WriteDebug("Inverse: " + quatToStr(glm::inverse(checkQuat)));
		glm::vec3 rotatedPoint = checkQuat * newCoords.cameraPos * glm::inverse(checkQuat);
		glm::vec3 rotatedFront = checkQuat * newCoords.cameraFront * glm::inverse(checkQuat);
		glm::vec3 rotatedUp = checkQuat * newCoords.cameraUp * glm::inverse(checkQuat);

		//WriteDebug("Rotated: " + vecToStr(rotatedFront));
		CameraCoords rotatedCoords = CameraCoords(rotatedPoint, newCoords.rotation, rotatedFront, rotatedUp);

		return rotatedCoords;
	}

	//Updates the current frame of the camera and the camera view. Runs once every RenderUpdate().
	void PlayCameraAction() 
	{
		if (currAction.ID != noAction.ID) 
		{
			if (currActionFrame < currAction.actionFrames.size())
			{
				offsetCoords = RotateCoords(currAction.actionFrames[currActionFrame]);
				UpdateCameraView();
				currActionFrame++;
			}
			else 
			{
				if (currAction.ID != noAction.ID) 
				{
					cameraCoords = cameraCoords + offsetCoords;
					currRotationAngle += currAction.actionFrames[currAction.actionFrames.size() - 1].rotation;
					//WriteDebug(vecToStr(cameraCoords.cameraFront));
					offsetCoords = CameraCoords();
					currAction = noAction;
				}
			}
		}
	}
};
