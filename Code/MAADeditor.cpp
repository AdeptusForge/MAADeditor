#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "SDL.h"
#include "Windows.h"
#include "IDControl.h"
#include "Debug.h"
#include "FileControl.h"
#include "Time.h"
#include "Physics.h"
#include "AudioControl.h"
#include "Input.h"
#include "Render.h"
#include "Editor.h"
#include "Map.h"


GLFWwindow* window;

//Starts up the entirety of the application
void InitializeTheMAADness() 
{
	IDStartup();
	FileControlStartup();

	//Startup functions
	window = RenderStartup();
	TimeStartup();
	InputControlStartup(window);
	EditorStartup(window);
	MapStartup("MapLoadTest");
	WriteDebug("MAADness Initialized");
}

//Quits out of the application, shuts down libraries safely, and frees up memory
int EndTheMAADness() 
{
	EditorShutdown();
	RenderShutdown();
	AudioControlQuit();
	SDL_Quit();
	glfwTerminate();
	return -1;
}

//Main while-loop exit conditions
bool WorkingProperly() 
{
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
		UpdateTime(window);
		glfwPollEvents();
	}

	return EndTheMAADness();
}