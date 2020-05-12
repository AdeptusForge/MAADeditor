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
#include "AnimEvents.h"
#include "memory"

//Stores texture by name and ID number instead of by data
//REFACTOR:: Store texture by data
struct Texture
{
	unsigned int ID;
	std::string name;
	Texture(unsigned int id, std::string name) : ID(id), name(name) {};
};

#pragma region Model Structs
struct Vertex
{
public:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoords;
	Vertex(glm::vec3 pos, glm::vec3 col, glm::vec2 tex) : position(pos), color(col), texCoords(tex) {};
};
struct Edge
{
	glm::vec3 edgeStart;
	glm::vec3 edgeEnd;
	glm::vec3 color;
	Edge(glm::vec3 start, glm::vec3 end, glm::vec3 col) : edgeStart(start), edgeEnd(end), color(col) {};
};
#pragma endregion

#pragma region Animation Structs
struct AnimEvent
{
private:
	std::string functionName;
	std::string variables;
	unsigned int activationFrame = 0;
public:
	AnimEvent(std::string data)
	{
		std::string word;
		char c1;
		std::istringstream buf(word);
		if (buf >> functionName >> c1 >> variables && c1 == '(')
		{
			//WriteDebug("Anim Event created for function: " + functionName);
		}
	};
	AnimEvent() {};
};
struct AnimFrame
{
private:
	glm::ivec2 textureChanges = { 0,0 };

public:
	glm::ivec2 GetTextureChanges() { return textureChanges; }
	void SetTextureChange(unsigned int textNum, unsigned int value)
	{
		if (textNum == 1) textureChanges.x = value;
		else if (textNum == 2) textureChanges.y = value;
	}

	AnimFrame() {};
};
struct AnimData
{
private:
	unsigned int length;
	std::vector<AnimFrame> frames;
	std::vector<Texture> textureLookup;
	std::vector<AnimEvent> events;
public:
	AnimFrame GetCurrFrame(unsigned int frame) { return frames[frame]; }
	Texture GetCurrTexture(unsigned int value) { return textureLookup[value]; }
	unsigned int GetLength() { return length; }
	AnimData(unsigned int l, std::vector<AnimFrame> frameVec, std::vector<Texture> loadtextures, std::vector<AnimEvent> eve) :
		length(l), frames(frameVec), textureLookup(loadtextures), events(eve) {
	};
	AnimData() {};
};
#pragma endregion

AnimData& LoadAnimData(std::string fileName);

struct ModelDataChunk
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Edge> edges;
	std::vector<Texture> textures;
	//std::vector<std::pair<int, int>> faces;
	ModelDataChunk(std::vector<Vertex>& verts, std::vector<unsigned int>& indices,
		std::vector<Edge>& edges, std::vector<Texture>& textures/*, std::vector<std::pair<int,int>> faces*/) :
		vertices(verts), indices(indices), edges(edges), textures(textures)/*, faces(faces)*/ {};
	ModelDataChunk() {};
};

unsigned char* LoadImageFile(FileType fileType, std::string fileName, int&, int&, int&);
void UnloadImageFile(unsigned char* image);
ModelDataChunk& Load3DModel(std::string fileName, FileType fileType);

//Main class for 3D models
//REFACTOR:: ??? 3D animation rework
class Model
{
	unsigned int currentFrame = 0;
	bool currentlyPlaying = false;
	AnimData currentAnim;
	//Loads raw image data, generates mipmaps, and then unloads the texture data.
	void PrepTexture(Texture &ref, bool startupBool) 
	{
		int width, height, nrChannels;
		if (startupBool) 
		{
			glGenTextures(1, &ref.ID);
		}
		glBindTexture(GL_TEXTURE_2D, ref.ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		unsigned char* texData = LoadImageFile(ImageFile, ref.name, width, height, nrChannels);
		//Create TextureLookup function to frontload textures.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);
		UnloadImageFile(texData);
	}
	
	//Binds a texture to an active opneGL memory location.
	void BindTextures(Shader shader)
	{
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			shader.setInt("texture" + std::to_string(i), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
		}
	}

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Edge> edges;
	std::vector<Texture> textures;
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
	//Plays the animation specified by setting the current animation abd resetting the current frame to 0
	void StartAnim(std::string data) 
	{
		this->currentAnim = LoadAnimData(data);
		currentlyPlaying = true;
		currentFrame = 0;
		//AnimFrame currFrame = currentAnim.GetCurrFrame(currentFrame);
		//WriteDebug(std::to_string(currFrame.GetTextureChanges().y));
	}
	
	//Animation update that occurs once per Draw() call.
	void PlayAnim()
	{
		if (currentlyPlaying) 
		{
			AnimFrame frame = currentAnim.GetCurrFrame(currentFrame);

			std::string newTexture = currentAnim.GetCurrTexture(frame.GetTextureChanges().y).name;
			if (textures[1].name != newTexture)
			{
				textures[1].name = newTexture;
				PrepTexture(textures[1], false);
			}
		}
		else {}
		currentFrame++;
		if (currentFrame >= currentAnim.GetLength()) 
		{
			currentlyPlaying = false;
			currentFrame = 0;
		}

	};
	//Preps the model for the next draw frame and rebinds current textures.
	void ModelRefresh(Shader shader)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		BindTextures(shader);
	}
	//Deletes old textures to prevent memory leaks.
	void ModelCleanup() 
	{
		for(int i=0; i< textures.size(); i++)
			glDeleteTextures(1, &textures[i].ID);
	}
	//Draws the model into the OpenGL space. Called once per RenderUpdate()
	void Draw(Shader shader)
	{
		//Draw
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
		if (currentFrame < currentAnim.GetLength())
		{
			PlayAnim();
		}
	};	
private:

	unsigned int VBO, EBO;
	//Initial model setup, including base textures and all pertinent buffers.
	void ModelSetup() 
	{
		#pragma region Load Textures
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			PrepTexture(textures[i],true);
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