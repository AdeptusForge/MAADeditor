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
#include "RNGRoll.h"

ModelDataChunk TestChunk;

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
			case (GLFW_KEY_I): 
			{RenderObject* model = GetRenderObject(1);
			model->objModel.StartAnim("AnimLoadTest");
				break; }
			case (GLFW_KEY_W): GetMapEntity(0)->Walk(GetMapEntity(0)->GetCurrentFacing(1)); break;
			case (GLFW_KEY_D): GetMapEntity(0)->Walk(GetMapEntity(0)->GetCurrentFacing(2)); break;
			case (GLFW_KEY_S): GetMapEntity(0)->Walk(GetMapEntity(0)->GetCurrentFacing(3)); break;
			case (GLFW_KEY_A): GetMapEntity(0)->Walk(GetMapEntity(0)->GetCurrentFacing(4)); break;

			case (GLFW_KEY_X): GetMapEntity(0)->Flip(); break;
			case (GLFW_KEY_Q): GetMapEntity(0)->Rotate(1); break;
			case (GLFW_KEY_E): GetMapEntity(0)->Rotate(0); break;

			case (GLFW_KEY_UP): cam->StartCameraAction(walkForward); break;
		}
	}
	if (action == GLFW_REPEAT) 
	{

		Camera* cam = FindCamera(1);
		switch (key)
		{

		}

	}
}

void SaveInputs()
{

}