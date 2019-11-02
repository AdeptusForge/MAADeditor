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
#include "physics.h"
#include "Components.h"

Camera* FindCamera(unsigned int);
void RenderShutdown();
void RenderUpdate(GLFWwindow*);
GLFWwindow* RenderStartup();

void ResetScreenSize(GLFWwindow*);

struct RenderObject 
{
public: 
	PhysicsTransform objLoc;
	Model objModel;

	RenderObject(PhysicsTransform pL, Model mod) : objLoc(pL), objModel(mod) {};
};
