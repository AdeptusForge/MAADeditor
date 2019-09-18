#include <glad/glad.h> 
#include "Render.h"
#include "SDL.h"
#include <GLFW/glfw3.h>
#include "Assets.h"
#include "FileControl.h"
#include <iostream>
#include "Shaders.h"

const unsigned int SCR_H = 800;
const unsigned int SCR_W = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, SCR_W, SCR_H);
};

void processInput(GLFWwindow* window) 
{
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

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_W, SCR_H, "MAADengine", NULL, NULL);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cerr << "GLAD failed to initialize\n";
	std::cout << "GLAD initialized successfully.\n";


	LoadCustomShader("PracticeVertexShader", "PracticeFragShader").use();

	return window;
}

void RenderUpdate() 
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float triangleVertices[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.8f,  0.8f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.6f, 0.6f, 0.0f,
		 0.8f, 0.6f, 0.0f,
		 0.6f, 0.8f, 0.0f
	};

	unsigned int triangleIndices[] = {  // note that we start from 0!
		0, 1, 3,
		1, 2, 4,
		5, 6, 7,
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s),
	// and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered
	// VBO as the vertex attribute's bound vertex buffer object so afterwards 
	// we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound 
	// element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally 
	// modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't 
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// draw our first triangle

	glBindVertexArray(VAO); 
	// seeing as we only have a single VAO there's no need to bind it every time, 
	// but we'll do so to keep things a bit more organized
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, (sizeof(triangleIndices) / sizeof(triangleIndices[0])) , GL_UNSIGNED_INT, 0);
	// glBindVertexArray(0); // no need to unbind it every time 
}