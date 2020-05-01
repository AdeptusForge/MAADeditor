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

struct CameraCoords
{
	glm::vec3 cameraPos;
	glm::vec3 rotation;
	glm::bvec3 rotationLock;
	CameraCoords(glm::vec3 pos, glm::vec3 rot, glm::bvec3 lock = glm::bvec3(false)):
		cameraPos(pos), rotation(rot), rotationLock(lock){};
	CameraCoords() {};
};

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
	CameraCoords(glm::vec3(0.0, 0.0, 7.0), glm::vec3(0,0,0))

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

class Camera 
{
private:
	float rotateDistance = 30;
	glm::vec3 currRotationAngle = glm::vec3(0, -90, 0);
	float rotateSpeed = 3;
	CameraCoords cameraCoords;
	glm::mat4 cameraView = glm::mat4(1.0f);
	unsigned int currActionFrame;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	CameraAction currAction;

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
		cameraFront = baseCameraFront;
		cameraUp = baseCameraUp;
		UpdateCameraView();
	};
	Camera() {};

	glm::mat4 GetCameraView() { return cameraView; }

	void UpdateCameraView() 
	{
		//WriteDebug("Front: " + std::to_string(cameraFront.x) + ", " + std::to_string(cameraFront.y) + ", " + std::to_string(cameraFront.z));
		//WriteDebug("Pos: " + std::to_string(cameraPos.x) + ", " + std::to_string(cameraPos.y) + ", " + std::to_string(cameraPos.z));
		//if(mode == FreeView)
		CameraCoords offsetAndBasePos;
		offsetAndBasePos.cameraPos = cameraCoords.cameraPos + offsetCoords.cameraPos;
		cameraView = glm::lookAt(offsetAndBasePos.cameraPos, offsetAndBasePos.cameraPos + cameraFront, cameraUp);
	}

	void MoveCamera(CameraCoords newCoords) 
	{
		cameraCoords.cameraPos = newCoords.cameraPos;
		UpdateCameraView();
	}

	void MoveCamera(glm::vec3 moveTo)
	{
		cameraCoords.cameraPos += moveTo;
		UpdateCameraView();
	}

	void StartCameraAction(CameraAction action) 
	{
		currAction = action;
		currActionFrame = 0;

	}
	CameraCoords RotateCoords(CameraCoords check) 
	{
		CameraCoords newCoords = check;
		//WriteDebug("CurrRotation: " + vecToStr(currRotationAngle) + " Frame Rotation: " + vecToStr(newCoords.rotation));
		glm::quat checkQuat = glm::radians((currRotationAngle + newCoords.rotation) * 0.5f);
		//WriteDebug("Quat: " + quatToStr(checkQuat));
		//WriteDebug("Inverse: " + quatToStr(glm::inverse(checkQuat)));
		glm::vec3 rotatedPoint = checkQuat * newCoords.cameraPos * glm::inverse(checkQuat);
		//WriteDebug("Rotated: " + vecToStr(rotatedPoint));
		CameraCoords rotatedCoords = CameraCoords(rotatedPoint, newCoords.rotation);

		return rotatedCoords;
	}


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
				currAction = noAction;
		}
	}
};
