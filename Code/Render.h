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
#include "Objects.h"

Camera* FindCamera(unsigned int);
void RenderShutdown();
void RenderUpdate(GLFWwindow*);
GLFWwindow* RenderStartup();

void ResetScreenSize(GLFWwindow*);

#define UNIVERSAL_RENDERSCALE glm::vec3(1)

struct RenderObject 
{
public: 
	unsigned int ID;
	PhysicsTransform objLoc;
	Model objModel;
	glm::vec3 objScale;


	RenderObject(PhysicsTransform pL, Model mod, unsigned int ID, glm::vec3 scale = UNIVERSAL_RENDERSCALE) 
		: objLoc(pL), objModel(mod), ID(ID), objScale(scale) {};
	RenderObject();
};

void GameToRenderConversion(GameObject);

RenderObject* GetRenderObject(int ID);