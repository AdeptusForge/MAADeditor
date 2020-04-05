#include <glad/glad.h> 
#include "Input.h"
#include <GLFW/glfw3.h>
#include <iterator>
#include <vector>
#include "Debug.h"
#include "AudioControl.h"
#include "Render.h"
#include "Camera.h"
#include "Physics.h"


std::vector<InputFrame> priorFrames;
std::vector<InputFrame>::iterator ptr;
MouseMode currMouseMode;

InputFrame currFrame;

bool isSelecting;

static void cursorPoitionCallback(GLFWwindow* window, double xPos, double yPos) 
{

}

void InputControlStartup(GLFWwindow* window)
{
	glfwSetKeyCallback(window, CollectInputs);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	for (int i = 1; i < 20; i++) 
	{
		ptr = priorFrames.begin();
		ptr = priorFrames.insert(ptr, InputFrame());
	}
}

void CollectInputs(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) 
	{
		Camera* cam = FindCamera(1);
		switch (key)
		{
			case (GLFW_KEY_ENTER): PlaySoundClip(SFX_SND, "soundTestGOT"); break;
			case (GLFW_KEY_K): ResetScreenSize(window); break;
			//case (GLFW_KEY_UP): cam->MoveCamera(glm::vec3(0.0f, 0.0f, -1.0f)); break;
			//case (GLFW_KEY_DOWN): cam->MoveCamera(glm::vec3(0.0f, 0.0f, 1.0f)); break;
			//case (GLFW_KEY_LEFT): cam->MoveCamera(glm::vec3(-1.0f, 0.0f, 0.0f)); break;
			//case (GLFW_KEY_RIGHT): cam->MoveCamera(glm::vec3(1.0f, 0.0f, 0.0f)); break;
			case (GLFW_KEY_LEFT): cam->RotateCamera(true); break;
			case (GLFW_KEY_RIGHT): cam->RotateCamera(false); break; 
		}
	}
	if (action == GLFW_REPEAT) 
	{

		Camera* cam = FindCamera(1);
		switch (key)
		{

			case (GLFW_KEY_LEFT): cam->RotateCamera(true); break;
			case (GLFW_KEY_RIGHT): cam->RotateCamera(false); break;
		}

	}
}


void SaveInputs()
{

}