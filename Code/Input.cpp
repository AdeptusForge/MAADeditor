#include <glad/glad.h> 
#include "Input.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iterator>
#include <vector>
#include "Debug.h"
#include "AudioControl.h"
#include "Render.h"
#include "Camera.h"
#include "Physics.h"
#include "RNGRoll.h"

ModelDataChunk TestChunk;

std::vector<InputFrame> priorFrames;
std::vector<InputFrame>::iterator ptr;
MouseMode currMouseMode;

InputFrame newFrame;
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
	for (int i = 1; i <= MAX_SAVED_INPUTFRAMES; i++) 
	{
		ptr = priorFrames.begin();
		ptr = priorFrames.insert(ptr, InputFrame());
	}
}

void CollectInputs(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	glm::bvec4 dirs = currFrame.RAW_DIRECTIONS();
	glm::bvec4 buttons = currFrame.RAW_BUTTONS();

	if (action == GLFW_PRESS || action == GLFW_REPEAT) 
	{
		if (key == GLFW_KEY_W)
		{
			dirs.w = true;
		}
		if (key == GLFW_KEY_D)
		{
			dirs.x = true;
		}
		if (key == GLFW_KEY_S)
		{
			dirs.y = true;
		}
		if (key == GLFW_KEY_A)
		{
			dirs.z = true;
		}


		if (key == GLFW_KEY_U) 
		{
			buttons.w = true;
		}
		if (key == GLFW_KEY_I)
		{
			buttons.x = true;
		}
		if (key == GLFW_KEY_O)
		{
			buttons.y = true;
		}
		if (key == GLFW_KEY_P)
		{
			buttons.z = true;
		}
	}
	if (action == GLFW_RELEASE) 
	{
		if (key == GLFW_KEY_W)
		{
			dirs.w = false;
		}
		if (key == GLFW_KEY_D)
		{
			dirs.x = false;
		}
		if (key == GLFW_KEY_S)
		{
			dirs.y = false;
		}
		if (key == GLFW_KEY_A)
		{
			dirs.z = false;
		}

		if (key == GLFW_KEY_U)
		{
			buttons.w = false;
		}
		if (key == GLFW_KEY_I)
		{
			buttons.x = false;
		}
		if (key == GLFW_KEY_O)
		{
			buttons.y = false;
		}
		if (key == GLFW_KEY_P)
		{
			buttons.z = false;
		}
	}
	newFrame = InputFrame(dirs, buttons);
}

void SaveInputs()
{
	ptr = priorFrames.begin();
	ptr = priorFrames.insert(ptr, currFrame);
	currFrame = newFrame;
	priorFrames.pop_back();
	//WriteDebug(std::to_string(newFrame.UP()) + " " + std::to_string(newFrame.RIGHT()));
	//if (newFrame.UP()) 
	//{
	//	WriteDebug("W check");
	//}
	//if (newFrame.RIGHT())
	//{
	//	WriteDebug("D check");
	//}
}

void RunInputs() 
{
	Camera* cam = FindCamera(1);
	WriteDebug(vecToStr(currFrame.RAW_DIRECTIONS()) + " " + vecToStr(currFrame.RAW_BUTTONS()));
	
	//switch (key)
	//{
	//case (GLFW_KEY_ENTER): PlaySoundClip(SFX_SND, "soundTestGOT"); break;
	//case (GLFW_KEY_I):
	//{RenderObject* model = GetRenderObject(1);
	//model->objModel.StartAnim("AnimLoadTest");
	//break; }
	//case (GLFW_KEY_W): GetMapEntity(0)->Walk(GetMapEntity(0)->GetCurrentFacing(1)); break;
	//case (GLFW_KEY_D): GetMapEntity(0)->Walk(GetMapEntity(0)->GetCurrentFacing(2)); break;
	//case (GLFW_KEY_S): GetMapEntity(0)->Walk(GetMapEntity(0)->GetCurrentFacing(3)); break;
	//case (GLFW_KEY_A): GetMapEntity(0)->Walk(GetMapEntity(0)->GetCurrentFacing(4)); break;

	//case (GLFW_KEY_X): GetMapEntity(0)->Flip(); break;
	//case (GLFW_KEY_Q): GetMapEntity(0)->Rotate(1); break;
	//case (GLFW_KEY_E): GetMapEntity(0)->Rotate(0); break;

	//case (GLFW_KEY_B): WriteDebug("pressed B"); break;
	//case (GLFW_KEY_Y): WriteDebug("pressed Y"); break;
	//case (GLFW_KEY_UP): cam->StartCameraAction(walkForward); break;

	//}
}