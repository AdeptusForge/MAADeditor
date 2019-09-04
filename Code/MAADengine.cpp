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

using namespace std;

GLFWwindow* window;
const unsigned int SCR_H = 800;
const unsigned int SCR_W = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, SCR_W, SCR_H);
};


void InitializeTheMAADness() 
{

	if (!glfwInit())
		cerr << "OpenGL failed to initialize\n";
	cout << "OpenGL initialized successfully.\n";

	/* Initialize the libraries */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		cerr << "SDL failed to initialize\n";
	cout << "SDL initialized successfully.\n";


	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT, 2, 2048) <0)
		cerr << "AudioMixer failed to initialize: " << Mix_GetError() << endl;
	cout << "AudioMixer initialized successfully.\n";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_W, SCR_H, "MAADengine", NULL, NULL);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		cerr << "GLAD failed to initialize\n";
	cout << "GLAD initialized successfully.\n";

	//Startup functions
	TimeStartup();
	FileControlStartup();

	WriteDebug("MAADness Initialized");
}
int EndTheMAADness() 
{
	Mix_Quit();
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
		glClear(GL_COLOR_BUFFER_BIT);

		/* Render here */

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return EndTheMAADness();
}