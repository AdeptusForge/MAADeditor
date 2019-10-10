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

unsigned int SCR_H = 800;
unsigned int SCR_W = 800;

int shaderID;

unsigned int texture1, texture2;
unsigned int VBO, VAO, EBO;


glm::mat4 trans = glm::mat4(1.0f);


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, SCR_W, SCR_H);
};

void ResetScreenSize(GLFWwindow* window) 
{
	glfwSetWindowSize(window, 800, 800);
}

void LoadBindTexture(std::string textureName, unsigned int &textureID, int &width, int &height, int &nrChannels)
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

	Shader ourShader = LoadCustomShader("PracticeVertexShader", "ColorTextureApplicator");
	shaderID = ourShader.ID;
	ourShader.use();
	
	//Texture Load Test
	int width, height, nrChannels;

	LoadBindTexture("SpaceShip1", texture1, width, height, nrChannels);
	LoadBindTexture("wallTest", texture2, width, height, nrChannels);

	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);







	return window;
}

void RenderShutdown()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	WriteDebug("Render Shutdown Successful");
}


void RenderUpdate(GLFWwindow* window)
{


	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
	};

	unsigned int indices[] = {  
		0, 1, 3, // first triangle
		1, 2, 3,  // second triangle


	};

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// binding
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	

	//Transform Test
	trans = glm::rotate(trans, glm::radians(5.0f), glm::vec3(0.0, 0.0, 1.0));

	unsigned int transformLoc = glGetUniformLocation(shaderID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	//glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//glm::mat4 view = glm::mat4(1.0f);
	//glm::mat4 projection = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//projection = glm::perspective(glm::radians(45.0f), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
	//// retrieve the matrix uniform locations
	//unsigned int modelLoc = glGetUniformLocation(shaderID, "model");
	//unsigned int viewLoc = glGetUniformLocation(shaderID, "view");
	//unsigned int projectionLoc = glGetUniformLocation(shaderID, "projection");

	//// pass them to the shaders (3 different ways)
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	//// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));





	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	 // texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	glBindVertexArray(VAO); 
	glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(indices[0])) , GL_UNSIGNED_INT, 0);
	
	
	// glBindVertexArray(0); // no need to unbind it every time 
	glfwSwapBuffers(window);

}