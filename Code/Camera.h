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
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp; //used to define the y+ axis for the camera
	glm::mat4 cameraView = glm::mat4(1.0f);

	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float cameraFov = 45.0f;
	Camera() {};

	Camera(unsigned int ID, glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov, CameraType type)
	{
		cameraID = ID;


		cameraPos = pos;
		cameraFront = front;
		cameraUp = up;
		fov = cameraFov;
		cameraType = type;

		
		UpdateCameraView();
	};

	void UpdateCameraView() 
	{
		if(mode == FreeView)
			cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	void MoveCamera(glm::vec3 moveTo)
	{
		cameraPos += moveTo;
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


		cameraPos = newPos;
		cameraFront = newFront;
		UpdateCameraView();

	}


};
