#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <string>
#include <iterator> 
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shaders.h"
#include "FileControl.h"

class Model
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Edge> edges;
	std::vector<Texture> textures;
	//std::vector<std::pair<int, int>> faces;
	unsigned int VAO;

	#pragma region Constructor
	Model(std::string modelName)
	{
		ModelDataChunk newModel = Load3DModel(modelName, ObjFile);

		this->vertices = newModel.vertices;
		this->indices = newModel.indices;
		this->edges = newModel.edges;
		this->textures = newModel.textures;
		//this->faces = newModel.faces;
		
		ModelSetup();
	}

	#pragma endregion

	void ModelRefresh()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	}
	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < textures.size(); i++) 
		{
			glActiveTexture(GL_TEXTURE0 + i);
			shader.setInt("texture" + std::to_string(i),i);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
		}
		//Draw
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	};


	//void FetchVisibleVerts() 
	//{
	//	GLuint tbo;
	//	glGenBuffers(1, &tbo);
	//	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 3, nullptr, GL_STATIC_READ);

	//	GLuint query;
	//	glGenQueries(1, &query);

	//	glEnable(GL_RASTERIZER_DISCARD);

	//	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

	//	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
	//		glBeginTransformFeedback(GL_TRIANGLES);
	//			glDrawArrays(GL_POINTS, 0, 8);
	//		glEndTransformFeedback();
	//	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

	//	glDisable(GL_RASTERIZER_DISCARD);

	//	glFlush();
	//	
	//	GLuint primitives;
	//	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitives);

	//	WriteDebug(std::to_string(primitives) + " Primitives");

	//	glm::vec3 feedback[8];
	//	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

	//	size_t n = sizeof(feedback) / sizeof(feedback[0]);
	//	for (int i = 0; i < n; i++) 
	//	{
	//		WriteDebug(std::to_string(feedback[i].x) + ", " + std::to_string(feedback[i].y));
	//	}

	//	glDeleteQueries(1, &query);
	//}
	

private:

	unsigned int VBO, EBO;

	void ModelSetup() 
	{
		#pragma region Load Textures
		int width, height, nrChannels;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glGenTextures(1, &textures[i].ID);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			unsigned char* texData;
			//Create TextureLookup function to frontload textures.
			texData = LoadImageFile(ImageFile, textures[i].name, width, height, nrChannels);
			if (texData)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
				WriteDebug("Texture failed to load:" + textures[i].name);
		}
		#pragma endregion

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		// ATTRIBUTES
		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));


		//FetchVisibleVerts();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
			&indices[0], GL_STATIC_DRAW);

		glBindVertexArray(1);

	}
};