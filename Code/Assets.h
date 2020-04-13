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
	unsigned int currentFrame;
	bool currentlyPlaying;

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

	void BindTextures(Shader shader)
	{
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			shader.setInt("texture" + std::to_string(i), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
		}
	}

	void PlayAnim(std::string anim)
	{
		if (currentlyPlaying) {}
		else {}

	};

	void ModelRefresh(Shader shader)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		BindTextures(shader);
	}

	void Draw(Shader shader)
	{
		//Draw
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	};	

private:

	unsigned int VBO, EBO;

	void ModelSetup() 
	{
		#pragma region Load Textures
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			int width, height, nrChannels;
			glGenTextures(1, &textures[i].ID);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			//Create TextureLookup function to frontload textures.
			unsigned char* texData = LoadImageFile(ImageFile, textures[i].name, width, height, nrChannels);
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