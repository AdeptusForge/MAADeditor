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
	glm::vec3 cameraUp; //used to define the y+ axis for the camera
	glm::mat4 cameraView = glm::mat4(1.0f);
};
struct CameraAction 
{
	std::vector<CameraCoords> actionFrames;
};
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
	float rotateDistance = 10;
	int currRotationAngle = 0;
	float rotateSpeed = 3;
public:
	unsigned int cameraID;

	CameraMode mode = FreeView;
	CameraType cameraType;
	CameraCoords baseCoords;

	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float cameraFov = 45.0f;

	Camera(unsigned int ID, glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov, CameraType type):
		cameraID(ID), cameraFov(fov), cameraType(type)
	{
		baseCoords.cameraPos = pos,
		baseCoords.cameraFront = front;
		baseCoords.cameraUp = up;
		UpdateCameraView();
	};
	Camera() {};

	void UpdateCameraView() 
	{
		//WriteDebug("Front: " + std::to_string(cameraFront.x) + ", " + std::to_string(cameraFront.y) + ", " + std::to_string(cameraFront.z));
		//WriteDebug("Pos: " + std::to_string(cameraPos.x) + ", " + std::to_string(cameraPos.y) + ", " + std::to_string(cameraPos.z));
		if(mode == FreeView)
			baseCoords.cameraView = glm::lookAt(baseCoords.cameraPos, baseCoords.cameraPos + baseCoords.cameraFront, baseCoords.cameraUp);
	}

	void MoveCamera(glm::vec3 moveTo)
	{
		baseCoords.cameraPos += moveTo;
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
		baseCoords.cameraFront = newFront;
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
		newPos.z = rotateDistance * cos(currRotationAngle * PI / 180);
		newFront.x = -sin(currRotationAngle * PI / 180);
		newFront.y = 0;
		newFront.z = -cos(currRotationAngle * PI / 180);


		baseCoords.cameraPos = newPos;
		baseCoords.cameraFront = newFront;
		UpdateCameraView();

	}


};
