#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Render.h"
#include "SDL.h"
#include "Assets.h"
#include "Debug.h"
#include "FileControl.h"
#include "Shaders.h"
#include "Camera.h"



Camera* FindCamera(unsigned int);
void RenderShutdown();
void RenderUpdate(GLFWwindow*);
GLFWwindow* RenderStartup();

void ResetScreenSize(GLFWwindow*);


class RenderModel 
{
private:
public: 


};
