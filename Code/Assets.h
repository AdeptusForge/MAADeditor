#pragma once

#include <string>
#include <iterator> 
#include <vector>


#include "FileControl.h"
#include "AnimEvents.h"
#include "memory"




//Main class for 3D models
//REFACTOR:: ??? 3D animation rework
class Model
{
private:
	glm::vec3 scale;
	unsigned int currentFrame = 0;
	bool currentlyPlaying = false;
	AnimData currentAnim;

	//Loads raw image data, generates mipmaps, and then unloads the texture data.
	void PrepTexture(Texture &ref, bool startupBool) 
	{
		if (startupBool) 
		{
			glGenTextures(1, &ref.ID);
		}
		glBindTexture(GL_TEXTURE_2D, ref.ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//Create TextureLookup function to frontload textures.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ref.Width(), ref.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ref.Data());
		glGenerateMipmap(GL_TEXTURE_2D);

		//BREAKS HERE.

		//UnloadImageFile(ref.Data());
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
	std::string modelStringName;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Edge> edges;
	std::vector<Texture> textures;
	unsigned int VAO;
	glm::vec3 Scale() { return scale; }

	#pragma region Constructor
	Model(std::string modelName, glm::vec3 newScale = glm::vec3(1))
	{
		WriteDebug("Constructing Model: " + modelName);
		modelStringName = modelName;
		if (modelName != "UI") 
		{
			ModelDataChunk newModel = *(ModelDataChunk*)GetLoadController()->RetrieveData(ObjFile, modelName, 0);
			this->scale = newScale;
			this->vertices = newModel.vertices;
			this->indices = newModel.indices;
			this->edges = newModel.edges;
			this->textures = newModel.textures;
			//this->faces = newModel.faces;

			ModelSetup();
		}
	}
	#pragma endregion
	//Plays the animation specified by setting the current animation and resetting the current frame to 0
	void StartAnim(std::string data) 
	{
		this->currentAnim = *(AnimData*)GetLoadController()->RetrieveData(AnimFile, data, 0);
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
			TextureAnimFrame frame = currentAnim.GetCurrFrame(currentFrame);

			std::string newTexture = currentAnim.GetCurrTexture(frame.GetTextureChanges().y).Name();
			if (textures[1].Name() != newTexture)
			{
				textures[1].Name() = newTexture;
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
	glm::mat4 ModelRefresh(Shader shader, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		BindTextures(shader);
		//glEnable(GL_SIGNED_NORMALIZED);
		glm::mat4 model = glm::mat4(1.0f);

		//SCALE then TRANSLATE then ROTATE. DON'T CHANGE THE ORDER
		model = glm::scale(model, scale);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		return model;
	}
	//Deletes old textures to prevent memory leaks.
	void ModelCleanup() 
	{
		for(int i=0; i< textures.size(); i++)
			glDeleteTextures(1, &textures[i].ID);
	}
	//Draws the model into the OpenGL space. Called once per RenderUpdate() per object.
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
			PrepTexture(textures[i], true);
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