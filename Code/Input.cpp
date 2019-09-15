#include "Input.h"
#include <GLFW/glfw3.h>
#include "Points.h"
#include <iterator>
#include <vector>
#include "Debug.h"
#include "AudioControl.h"

std::vector<InputFrame> priorFrames;
std::vector<InputFrame>::iterator ptr;

InputFrame currFrame;


void InputControlStartup(GLFWwindow* window)
{
	glfwSetKeyCallback(window, CollectInputs);
	for (int i = 1; i < 20; i++) 
	{
		ptr = priorFrames.begin();
		ptr = priorFrames.insert(ptr, InputFrame());
	}
}

void CollectInputs(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) 
	{
		switch (key)
		{
			case (GLFW_KEY_ENTER): WriteDebug("Sound Should Play"); PlaySoundClip(SFX_SND, "soundTestGOT"); break;
			
		}
	}
}


void SaveInputs()
{

}