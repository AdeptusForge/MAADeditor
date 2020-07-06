#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Render.h"
#include "SDL.h"
#include "Assets.h"
#include "FileControl.h"
#include <iostream>
#include "Shaders.h"
#include "Camera.h"
#include "Assets.h"
#include "Physics.h"
#include "editor.h"
#include "UIControl.h"
#include <algorithm>
#include <iterator>

const unsigned int SCR_H = 360;
const unsigned int SCR_W = 640;

glm::ivec2 screenDimensions;

Shader mainShader;
Shader uiShader;
Camera ourCamera;

std::vector<Camera*> allCameras;
std::vector<int>::iterator camIT;
std::vector<RenderObject> allModels;
std::vector<RenderObject> allUIModels;

MAAD_UIContext mainUI;
std::vector<MAAD_UIElement*> UIelements;
struct IDFinder {

	IDFinder(unsigned int const& id) : ID(id) { }

	bool operator () (const RenderObject el) const { return el.ID == ID; }

private:
	unsigned int ID;
};

//Converts a gameobject into a renderobject for use during rendering.
void GameToRenderConversion(GameObject obj)
{
	for (int i = 0; i < obj.models.size(); i++)
	{
		PhysicsTransform renderTrans = obj.models[i].modelOffset + obj.transform;
		Model renderModel = obj.models[i].viewModel;
		allModels.push_back(RenderObject(renderTrans, renderModel, 15));
	}
}

//Converts a MAAD_UIElement into a renderobject for use during rendering.
void UIElementToRenderConversion(MAAD_UIElement* element, Camera* target) 
{
	//WriteDebug(vecToStr(target->GetCameraCoords().cameraPos));
	
	PhysicsTransform renderTrans = PhysicsTransform(element->CalculateElementOffset(target), glm::vec3(0), true);
	allUIModels.push_back(RenderObject(renderTrans, element->GetModel(), 15));
}

void UpdateContext(MAAD_UIContext* ui, Shader shader)
{
	UIelements = ui->elementPTRs;
	for (int i = 0; i < ui->elementPTRs.size(); i++)
		if (ui->elementPTRs[i]->Active() == true) 
		{
			ui->elementPTRs[i]->UpdateElement(shader, ui->GetTargetCamera());
		}
};

//Gets a camera from the list of allcameras.
Camera* FindCamera(unsigned int camID) 
{
	Camera* found = nullptr;
	for (int i = 0; i != allCameras.size(); i++) 
	{
		if (allCameras.at(i)->cameraID == camID)
			found = allCameras.at(i);
	}
	return found;
}

glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

//readjusts the view on the screen to fit the window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	projection = glm::perspective(glm::radians(ourCamera.cameraFov), ((float)width / (float)height), 0.1f, 100.0f);
	screenDimensions = glm::vec2(width,height);
};

//Gets a specific renderobject from the list of allModels
RenderObject* GetRenderObject(int ID) 
{
	if (!allModels.empty())
	{
		RenderObject* p;
		std::vector<RenderObject>::iterator it = std::find_if(allModels.begin(), allModels.end(), IDFinder(ID));
		if (!std::none_of(allModels.begin(), allModels.end(), IDFinder(ID)))
			p = &(*it);
		else
		{
			WriteDebug("No RenderObject with matching ID");
			return nullptr;
		}
		return p;

	}
	WriteDebug("No AllModels");
	return nullptr;
}

//Readjusts the size of the window when dragging the edges of it.
void ResetScreenSize(GLFWwindow* window) 
{
	glfwSetWindowSize(window, 800, 800);
	screenDimensions = glm::vec2(800, 800);
}

//Initializes an OpenGL window.
GLFWwindow* RenderStartup() 
{
	GLFWwindow* window;
	/* Initialize the libraries */
	if (!glfwInit())
		std::cerr << "OpenGL failed to initialize\n";
	std::cout << "OpenGL initialized successfully.\n";
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		std::cerr << "SDL failed to initialize\n";
	std::cout << "SDL initialized successfully.\n";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_W, SCR_H, "MAADeditor", NULL, NULL);
	screenDimensions = glm::vec2(SCR_W, SCR_H);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cerr << "GLAD failed to initialize\n";
	std::cout << "GLAD initialized successfully.\n";

	mainShader = LoadCustomShader("PracticeVertexShader", "ColorTextureApplicator", "PracticeGeometryShader");
	mainShader.use();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	uiShader = LoadCustomShader("UIVertShader", "ColorTextureApplicator", "PracticeGeometryShader");
	uiShader.use();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ourCamera = Camera( 1,
		glm::vec3(0.0f, 5.0f, 0.0f), 
		glm::vec3(0.0f, 0.0f, 1.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		45.0f, Perspective);
	allCameras.insert(allCameras.end(), &ourCamera);

	view = ourCamera.GetCameraView();
	projection = glm::perspective(glm::radians(ourCamera.cameraFov), ((float)SCR_W / (float)SCR_H), 0.1f, 100.0f);
	
	mainUI.UIStartup();

	Model testModel = Model("TileFloorCube");

	for (int x = 0; x < 5; x++) 
	{
		for (int y = 0; y < 5; y++)
		{
			glm::ivec3 newVec = glm::ivec3(((x * 10) - 40) * MAX_DECIMAL_PRECISION, 0, ((y * 10)) * MAX_DECIMAL_PRECISION);
			RenderObject newRO = RenderObject(PhysicsTransform(newVec, glm::ivec3(0), false), testModel, 1);
			allModels.push_back(newRO);
		}
	}
	UIelements = mainUI.elementPTRs;
	for (int i = 0; i < UIelements.size(); i++)
	{
		UIElementToRenderConversion(UIelements[i], mainUI.GetTargetCamera());
	}

	return window;
}

//Shuts down all OpenGL operations
//REFACTOR:: Actually make this do something.
void RenderShutdown()
{
	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);*/
	WriteDebug("Render Shutdown Successful");
}

//Updates the window's render. Called ones per render frame(1/60th of a second.)
void RenderUpdate(GLFWwindow* window)
{
	mainShader.use();
	ourCamera.UpdateCamera();
	// pass them to the shaders
	mainShader.setMat4("view", ourCamera.GetCameraView());
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	mainShader.setMat4("projection", projection);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Scene Rendering
	for (int i = 0; i < allModels.size(); i++)
	{
		//float angle = 20.0f * i;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		mainShader.setMat4("model", allModels[i].objModel.ModelRefresh(
			mainShader, allModels[i].objLoc.GetWorldPosition(), allModels[i].objScale, allModels[i].objLoc.GetWorldRotation()));
		allModels[i].objModel.Draw(mainShader);
	}
	uiShader.use();
	uiShader.setVec3("cameraPos", ourCamera.GetCameraCoords().cameraPos);
	uiShader.setMat4("view", ourCamera.GetCameraView());
	glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(screenDimensions.x), 0.0f, static_cast<float>(screenDimensions.y), -1.0f, 1.0f);
	uiShader.setMat4("projection", projection);
	UpdateContext(&mainUI, mainShader);
	//UI Rendering
	for (int i = 0; i < allUIModels.size(); i++)
	{
		uiShader.setMat4("model", allUIModels[i].objModel.ModelRefresh(
			uiShader, allUIModels[i].objLoc.GetWorldPosition(), allUIModels[i].objScale, allUIModels[i].objLoc.GetWorldRotation()));
		allUIModels[i].objModel.Draw(mainShader);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//EditorUpdate(window);
	glfwSwapBuffers(window);
	glFlush();
}