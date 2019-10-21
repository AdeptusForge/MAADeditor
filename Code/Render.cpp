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

unsigned int SCR_H = 800;
unsigned int SCR_W = 800;

Shader ourShader;
Camera ourCamera;

std::vector<Camera*> allCameras;
std::vector<int>::iterator camIT;
std::vector<Model> allModels;


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

void ResetScreenSize(GLFWwindow* window) 
{
	glfwSetWindowSize(window, 800, 800);
}

void LoadandBindTexture(std::string textureName, unsigned int& textureID, int& width, int& height, int& nrChannels)
{
	unsigned char* texData;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	texData = LoadImageFile(ImageFile, textureName, width, height, nrChannels);
	if (texData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		WriteDebug("Texture failed to load:" + textureName);
	}

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

	window = glfwCreateWindow(SCR_W, SCR_H, "MAADengine", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cerr << "GLAD failed to initialize\n";
	std::cout << "GLAD initialized successfully.\n";

	ourShader = LoadCustomShader("PracticeVertexShader", "ColorTextureApplicator");
	ourShader.use();
	
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ourCamera = Camera( 1,
		glm::vec3(0.0f, 0.0f, 10.0f), 
		glm::vec3(0.0f, 0.0f, -1.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		45.0f, Perspective);
	allCameras.insert(allCameras.end(), &ourCamera);

	view = ourCamera.cameraView;
	projection = glm::perspective(glm::radians(ourCamera.cameraFov), ((float)SCR_W / (float)SCR_H), 0.1f, 100.0f);

	for (int i = 0; i < 10; i++)
	{
		Model newModel = Model("ModelLoadTest", ourShader);
		allModels.push_back(newModel);
	}
	
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
	// world space positions of our objects
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// pass them to the shaders
	ourShader.setMat4("model", model);
	ourShader.setMat4("view", ourCamera.cameraView);

	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	ourShader.setMat4("projection", projection);



	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < allModels.size(); i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		ourShader.setMat4("model", model);
		allModels[i].Draw();
	}
	ourShader.use();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(VAO); 
	

	//for (unsigned int i = 0; i < 10; i++)
	//{


	//	// calculate the model matrix for each object and pass it to shader before drawing


	//}


	glfwSwapBuffers(window);

}