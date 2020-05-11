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
#include <algorithm>
#include <iterator>

unsigned int SCR_H = 360;
unsigned int SCR_W = 640;

Shader ourShader;
Camera ourCamera;

std::vector<Camera*> allCameras;
std::vector<int>::iterator camIT;
std::vector<RenderObject> allModels;

struct IDFinder {

	IDFinder(unsigned int const& id) : ID(id) { }

	bool operator () (const RenderObject el) const { return el.ID == ID; }

private:
	unsigned int ID;
};

void GameToRenderConversion(GameObject obj)
{
	for (int i = 0; i < obj.models.size(); i++)
	{
		PhysicsTransform renderTrans = obj.models[i].modelOffset + obj.transform;
		Model renderModel = obj.models[i].viewModel;
		allModels.push_back(RenderObject(renderTrans, renderModel, 15));
	}
}

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	projection = glm::perspective(glm::radians(ourCamera.cameraFov), ((float)width / (float)height), 0.1f, 100.0f);

};

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

void ResetScreenSize(GLFWwindow* window) 
{
	glfwSetWindowSize(window, 800, 800);
}

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
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cerr << "GLAD failed to initialize\n";
	std::cout << "GLAD initialized successfully.\n";

	ourShader = LoadCustomShader("PracticeVertexShader", "ColorTextureApplicator", "PracticeGeometryShader");
	ourShader.use();
	
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


	Model newModel = Model("TileFloorCube");
	Model testModel = Model("TileFloorCube");

	for (int x = 0; x < 5; x++) 
	{
		for (int y = 0; y < 5; y++)
		{
			RenderObject newRO = RenderObject(PhysicsTransform(glm::vec3(x * 10 * MAX_DECIMAL_PRECISION, 0, y * 10 * MAX_DECIMAL_PRECISION), glm::vec3(0)), testModel, 1);
			allModels.push_back(newRO);
		}
	}
	WriteDebug(allModels.size());
	//LoadAnimData("AnimLoadTest");
	//newModel = Model("ObjLoadTest");
	//allModels.push_back(RenderObject(PhysicsTransform(glm::vec3(0), glm::vec3(0)), newModel, 1));

	return window;
}

void RenderShutdown()
{
	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);*/
	WriteDebug("Render Shutdown Successful");
}

void RenderUpdate(GLFWwindow* window)
{
	
	ourCamera.PlayCameraAction();
	// pass them to the shaders
	ourShader.setMat4("model", model);
	ourShader.setMat4("view", ourCamera.GetCameraView());
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	ourShader.setMat4("projection", projection);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ourShader.use();

	for (int i = 0; i < allModels.size(); i++)
	{
		allModels[i].objModel.ModelRefresh(ourShader);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, allModels[i].objLoc.GetWorldPosition());
		//float angle = 20.0f * i;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		ourShader.setMat4("model", model);
		allModels[i].objModel.Draw(ourShader);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	EditorUpdate(window);
	glfwSwapBuffers(window);
}