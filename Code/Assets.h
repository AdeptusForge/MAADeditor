#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <string>
#include <list> 
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
	unsigned int VAO;

	#pragma region Constructor
	Model(std::string modelName)
	{
		ModelDataChunk newModel = Load3DModel(modelName);

		this->vertices = newModel.vertices;
		this->indices = newModel.indices;
		this->edges = newModel.edges;
		this->textures = newModel.textures;
		
		ModelSetup();
	}
	#pragma endregion

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
			{
				WriteDebug("Texture failed to load:" + textures[i].name);
			}
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
			&indices[0], GL_STATIC_DRAW);

		glBindVertexArray(1);
	}
};

//class SpriteSheet 
//{
//private:
//
//public:
//
//	SDL_Surface spriteSheet;
//	int frameCount;
//
//	//height x width
//	Point2Int frameDimensions;
//
//	//Constructors
//	SpriteSheet(SDL_Surface sheet, int count, Point2Int dim) : spriteSheet(sheet), frameCount(count), frameDimensions(dim) {};
//
//	vector<SDL_Surface> ReturnSprites() 
//	{
//		vector<SDL_Surface> sprites;
//		//TODO: add in sprite sheet support.
//	};
//};
//
//class AnimFrame2D
//{
//private:
//
//public:
//	SDL_Surface frame;
//	int duration;
//	//TODO: add possible animation tie-ins like scripts etc. & figure out method to remove duration
//
//	AnimFrame2D(SDL_Surface image, int dur = 1)
//	{
//		frame = image;
//		duration = dur;
//	}
//};
//
//class Animation2D 
//{
//private:
//
//public:
//	vector <AnimFrame2D> animData;
//
//	Animation2D(vector<SDL_Surface> imageSet) 
//	{
//		animData = vector <AnimFrame2D>();
//
//		for (int i=0; i < imageSet.size(); i++) 
//		{
//			animData.insert(animData.end(),AnimFrame2D(imageSet.at(i), 1));
//		}
//	};
//};