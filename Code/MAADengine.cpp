#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "SDL.h"
#include "Windows.h"
#include "Debug.h"
#include "FileControl.h"
#include "Time.h"
#include "Physics.h"
#include "AudioControl.h"
#include "Input.h"
#include "Render.h"

GLFWwindow* window;


void InitializeTheMAADness() 
{

	//Startup functions
	window = RenderStartup();
	TimeStartup();
	FileControlStartup();
	AudioControlStartup();
	InputControlStartup(window);

	WriteDebug("MAADness Initialized");
}
int EndTheMAADness() 
{
	AudioControlQuit();
	SDL_Quit();
	glfwTerminate();
	return -1;
}
//Main while-loop exit conditions
bool WorkingProperly() 
{
	if (!TimeWorkingProperly()) { WriteDebug("Shutdown -- Time Broke"); return false; }
	if (glfwWindowShouldClose(window)) { WriteDebug("Shutdown -- Window Closed"); return false; }
	else return true;
}


int main(int argc, char **argv)
{
	InitializeTheMAADness();
	//Don't move this if statement; it needs to be here for some reason...
	if (!window) return EndTheMAADness();

	while (WorkingProperly())
	{
		UpdateTime();

		glfwSwapBuffers(window);
		glfwPollEvents();
		SaveInputs();
	}

	return EndTheMAADness();
}