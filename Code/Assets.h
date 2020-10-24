#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <string>
#include <iterator> 
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "FileControl.h"
#include "AnimEvents.h"
#include "memory"
#include "sstream"

//Custom shader class that is passed through OpenGL.
//REFACTOR:: Geometry Shader functionality.
class Shader
{
public:
	// the program ID
	unsigned int ID;

	Shader() {}
	// constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;


		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			gShaderFile.open(geometryPath);
			std::stringstream vShaderStream, fShaderStream, gShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			gShaderStream << gShaderFile.rdbuf();

			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			gShaderFile.close();

			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			geometryCode = gShaderStream.str();

			//WriteDebug("Shader Read successfully");
		}

		catch (std::ifstream::failure e)
		{
			WriteDebug("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		const char* gShaderCode = geometryCode.c_str();


		// 2. compile shaders
		unsigned int vertex, fragment, geometry;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		// geometry Shader
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");


		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glAttachShader(ID, geometry);

		const GLchar* feedbackVaryings[] = { "fVerts" };
		glTransformFeedbackVaryings(ID, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void use()
	{
		glUseProgram(ID);
	}
#pragma region Utility Uniform Functions

	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
#pragma endregion

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				WriteDebug("ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- ");
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				WriteDebug("ERROR::PROGRAM_LINKING_ERROR of type: " + type +"\n" + infoLog + "\n -- --------------------------------------------------- -- ");
			}
		}
	}
};

Shader LoadCustomShader(std::string, std::string, std::string);

unsigned char* LoadImageFile(FileType fileType, std::string fileName, int&, int&, int&);

//Stores texture data via STBI data pointer
class Texture
{
private:
	int width, height, nrChannels;

	std::string name;
	unsigned char* data;

public:
	unsigned int ID;

	std::string Name() { return name; }
	unsigned char* Data() { return data; }
	int Width() { return width; }
	int Height() { return height; }
	int NRChannels() { return nrChannels; }

	Texture(unsigned int id, std::string name) : ID(id), name(name) 
	{
		data = LoadImageFile(ImageFile, name, width, height, nrChannels);
	};
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
struct TextureAnimFrame
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

	TextureAnimFrame() {};
};

struct AnimData
{
private:
	unsigned int length;
	std::vector<TextureAnimFrame> frames;
	std::vector<Texture> textureLookup;
	std::vector<AnimEvent> events;
public:
	TextureAnimFrame GetCurrFrame(unsigned int frame) { return frames[frame]; }
	Texture GetCurrTexture(unsigned int value) { return textureLookup[value]; }
	unsigned int GetLength() { return length; }
	AnimData(unsigned int l, std::vector<TextureAnimFrame> frameVec, std::vector<Texture> loadtextures, std::vector<AnimEvent> eve) :
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

void UnloadImageFile(unsigned char* image);
ModelDataChunk& Load3DModel(std::string fileName, FileType fileType);

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

		unsigned char* texData = ref.Data();
		//Create TextureLookup function to frontload textures.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ref.Width(), ref.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
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
			ModelDataChunk newModel = Load3DModel(modelName, ObjFile);
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