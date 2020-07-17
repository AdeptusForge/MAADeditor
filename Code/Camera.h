#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include "Objects.h"
#include "IDControl.h"


//#pragma region Hardcoded CameraActions
//const CameraAction noAction = CameraAction(0, std::vector<CameraCoords>{});
//const CameraAction walkForward = CameraAction(1, std::vector<CameraCoords> 
//{
//	CameraCoords(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0,0,0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 2.0), glm::vec3(0,0,0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0,0,0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 4.0), glm::vec3(0,0,0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0,0,0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 6.0), glm::vec3(0,0,0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 7.0), glm::vec3(0,0,0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 8.0), glm::vec3(0, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 9.0), glm::vec3(0, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 10.0), glm::vec3(0, 0, 0))
//
//});
//const CameraAction turnRight = CameraAction(2, std::vector<CameraCoords>
//{
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -15, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -30, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -45, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -60, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -75, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -90, 0))
//});
//const CameraAction turnLeft = CameraAction(3, std::vector<CameraCoords>
//{
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 15, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 30, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 45, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 60, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 75, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 90, 0))
//});
//const CameraAction turn180 = CameraAction(4, std::vector<CameraCoords>
//{
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 30, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 60, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 90, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 120, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 150, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 180, 0))
//});
//
//const CameraAction LookUp = CameraAction(5, std::vector<CameraCoords>
//{
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-15, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-30, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-45, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-60, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-75, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-90, 0, 0))
//});
//const CameraAction LookMiddleFromUp = CameraAction(6, std::vector<CameraCoords>
//{
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(15, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(30, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(45, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(60, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(75, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(90, 0, 0))
//});
//const CameraAction LookMiddleFromDown = CameraAction(7, std::vector<CameraCoords>
//{
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-90, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-75, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-60, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-45, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-30, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-15, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, 0))
//});
//const CameraAction LookDown = CameraAction(8, std::vector<CameraCoords>
//{
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-15, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-30, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-45, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-60, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-75, 0, 0)),
//	CameraCoords(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-90, 0, 0))
//});

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

	Camera( glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov, CameraType type, int ID = -1):
		cameraID(GetIDController("Cameras")->CreateNewID(ID)), cameraFov(fov), cameraType(type)
	{
		cameraPos = pos,
		UpdateCameraView();
	};
	Camera() {};

	glm::mat4 GetCameraView() { return cameraView; }

	void RotateCamera(glm::vec3 newRot) 
	{
		rotVec = rotVec+ newRot;
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
