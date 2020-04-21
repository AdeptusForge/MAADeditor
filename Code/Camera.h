#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#define PI 3.14159265

struct CameraCoords
{
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //used to define the y + axis for the camera
	CameraCoords(glm::vec3 pos, glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)):
		cameraPos(pos), cameraFront(front), cameraUp(up){};
	CameraCoords() {};

	CameraCoords operator + (CameraCoords const& obj) {
		CameraCoords res;
		res.cameraPos = cameraPos + obj.cameraPos;
		res.cameraUp = obj.cameraUp;
		res.cameraFront = obj.cameraFront;
		return res;
	}
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
	CameraCoords(glm::vec3(0.0, 0.0, -1.0)),
	CameraCoords(glm::vec3(0.0, 0.0, -2.0)),
	CameraCoords(glm::vec3(0.0, 0.0, -3.0)),
	CameraCoords(glm::vec3(0.0, 0.0, -4.0))


	//CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0)),
	//CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0)),
	//CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0)),
	//CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0)),
	//CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0)),
	//CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0)),
	//CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0)),
	//CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0)),
	//CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0)),
	//CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0)),
	//CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0))

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
	int currRotationAngle = 0;
	float rotateSpeed = 3;
	CameraCoords cameraCoords;
	glm::mat4 cameraView = glm::mat4(1.0f);
	unsigned int currActionFrame;
public:
	unsigned int cameraID;
	CameraMode mode = FreeView;
	CameraType cameraType;
	CameraCoords offsetCoords = CameraCoords(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	CameraCoords GetCameraCoords() { return cameraCoords; }
	CameraAction currAction = noAction;

	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float cameraFov = 45.0f;

	Camera(unsigned int ID, glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov, CameraType type):
		cameraID(ID), cameraFov(fov), cameraType(type)
	{
		cameraCoords.cameraPos = pos,
		cameraCoords.cameraFront = front;
		cameraCoords.cameraUp = up;
		UpdateCameraView();
	};
	Camera() {};

	glm::mat4 GetCameraView() { return cameraView; }

	void UpdateCameraView() 
	{
		//WriteDebug("Front: " + std::to_string(cameraFront.x) + ", " + std::to_string(cameraFront.y) + ", " + std::to_string(cameraFront.z));
		//WriteDebug("Pos: " + std::to_string(cameraPos.x) + ", " + std::to_string(cameraPos.y) + ", " + std::to_string(cameraPos.z));
		//if(mode == FreeView)
		CameraCoords offsetAndBaseCoords = cameraCoords + offsetCoords;
		cameraView = glm::lookAt(offsetAndBaseCoords.cameraPos, offsetAndBaseCoords.cameraPos + offsetAndBaseCoords.cameraFront, offsetAndBaseCoords.cameraUp);
	}

	void MoveCamera(CameraCoords newCoords) 
	{
		cameraCoords.cameraFront = newCoords.cameraFront;
		cameraCoords.cameraPos = newCoords.cameraPos;
		cameraCoords.cameraUp = newCoords.cameraUp;
		UpdateCameraView();
	}

	void MoveCamera(glm::vec3 moveTo)
	{
		cameraCoords.cameraPos += moveTo;
		UpdateCameraView();
	}

	void RotateCamera(bool dir)
	{
		glm::vec3 newFront;
		if (dir) currRotationAngle -= rotateSpeed;
		else currRotationAngle += rotateSpeed;
		newFront.x = sin(currRotationAngle * PI / 180);
		newFront.y = 0;
		newFront.z = -cos(currRotationAngle * PI / 180);
		cameraCoords.cameraFront = newFront;
		UpdateCameraView();
	}

	// + is right, - is left
	void RotateAroundOrigin(bool dir)
	{

		glm::vec3 newPos;
		glm::vec3 newFront;

		if (dir) currRotationAngle -= rotateSpeed;
		else currRotationAngle += rotateSpeed;

		newPos.x = rotateDistance * sin(currRotationAngle * PI / 180);
		newPos.y = 5;
		newPos.z = rotateDistance * cos(currRotationAngle * PI / 180);
		newFront.x = -sin(currRotationAngle * PI / 180);
		newFront.y = 0;
		newFront.z = -cos(currRotationAngle * PI / 180);


		cameraCoords.cameraPos = newPos;
		cameraCoords.cameraFront = newFront;
		UpdateCameraView();
	}

	void StartCameraAction(CameraAction action) 
	{
		currAction = action;
		currActionFrame = 0;
	}

	void PlayCameraAction() 
	{
		if (currAction.ID != noAction.ID) 
		{
			currActionFrame++;
			WriteDebug("Played CameraAction");
			if (currActionFrame < currAction.actionFrames.size())
			{
				offsetCoords = currAction.actionFrames[currActionFrame];
				UpdateCameraView();
			}
			else
				currAction = noAction;
		}
	}
};
