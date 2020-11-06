#pragma once
#include "framework.h"
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h> 
#include "SDL_mixer.h"
#include "Vector"
#include "array"
#include "iterator"
#include "IDcontrol.h"
#include "map"
#include "deque"
#include "sstream"

#ifndef MAX_MODEL_TEXTURES
const unsigned int MAX_MODEL_TEXTURES = 8;
#endif // !MAX_MODEL_TEXTURES

enum FileType
{
	None,
	ImageFile,
	AudioFile,
	AnimFile,
	SaveFile,
	PrefabFile,
	LevelFile,
	VertexShaderFile,
	FragmentShaderFile,
	GeometryShaderFile,
	ObjFile,
	ObjTextureFile
};

void SaveActiveFile(FileType fileType, std::string fileName, std::string data);

bool VerifyFileOrFolder(std::string filePath);
std::string FetchPath(FileType fileType, std::string fileName, bool saving);
void FileControlStartup();





unsigned char* LoadImageFile(FileType, std::string, int&, int&, int&);
void UnloadImageFile(unsigned char* image);


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
				WriteDebug("ERROR::PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- ");
			}
		}
	}
};

Shader LoadCustomShader(std::string, std::string, std::string);

#pragma region Map Structs
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
	unsigned char* Data() { if(data) return data; }
	int Width() { return width; }
	int Height() { return height; }
	int NRChannels() { return nrChannels; }
	
	void SetData(unsigned char* datum) 
	{
		if (datum == nullptr || !datum)
		{
			WriteDebug("recieved no data");
		}
		else
			data = datum;
	}
	Texture(unsigned int id, std::string name, unsigned char* datum) : ID(id), name(name) 
	{
		SetData(datum);
	};
	Texture(unsigned int id, std::string name, int w, int h, int n) : ID(id), name(name), width(w), height(h), nrChannels(n) {};
	Texture(unsigned int id, std::string name,unsigned char* datum , int w, int h, int n) : ID(id), name(name), data(datum), width(w), height(h), nrChannels(n) {};

};

enum TileFeature
{
	Empty = 0,
	Wall = 1,
	Ceiling = 2,
	Floor = 3,
};

struct TileFeatures
{
	TileFeature north;
	TileFeature east;
	TileFeature south;
	TileFeature west;
	TileFeature upward;
	TileFeature downward;
};
 
struct MapTile
{
private:
	glm::ivec3 mapPos;
	TileFeatures features;
	std::string tileModel;
	MapDirection modelOrientation = North; //1 = north, 2 east, 3 south, 4 west
	std::vector<std::string> functionTriggers;
public:
	MapTile(glm::ivec3 pos, TileFeatures f, std::string modelFile, MapDirection orient, std::vector<std::string> functionNames)
		: mapPos(pos), features(f), tileModel(modelFile), modelOrientation(orient), functionTriggers(functionNames) {};
	MapTile() {};
	TileFeature getTileFeature(MapDirection dir)
	{
		if (dir == North)
			return features.north;
		else if (dir == East)
			return features.east;
		else if (dir == South)
			return features.south;
		else if (dir == West)
			return features.west;
		else if (dir == Up)
			return features.upward;
		else if (dir == Down)
			return features.downward;
	}
	std::string getTileModel() { return tileModel; }
};

struct MapDataChunk
{
	glm::ivec3 mapSize;
	std::vector<MapTile> tileMap;
	MapDataChunk(unsigned int& xSize, unsigned int& ySize, unsigned int& zSize, std::vector<MapTile>& tiles)
		: mapSize(glm::ivec3(xSize, ySize, zSize)), tileMap(tiles) {};
	MapDataChunk() {};
};
#pragma endregion
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

AnimData& LoadAnimData(FileType fType, std::string fileName);
ModelDataChunk Load3DModel(FileType fileType, std::string fileName);
MapDataChunk& LoadMapData(std::string);


class LoadRequestController 
{
	std::map<std::string, ModelDataChunk> modelDataMap;
	std::map<std::string, MapDataChunk> mapDataMap;
	std::map<std::string, Texture> textureDataMap;

	std::map<std::string, AnimData> animDataMap;

	//std::map< INSERT AUDIO CLASS HERE, std::string> audioDataMap;
public:
	class LoadRequest
	{
	public:
		struct RequestData
		{
			std::string fileName;
			FileType loadType;
			//Priority Levels:
			//0 - This is a standard load request, created from a script during a loading screen, silent load section, or specialized script.
			//Always filled unless an error occurs within the LoadRequestController.
			//1 - This is an unusual load request that was created from something that needs the data directly, and is not filled, as it is 
			//not meant to occur. Use this version for bug checking.
			//2 - This is an unusual load request that comes with high priority. This occurs when something has a direct need for data, and
			//and will cause UNDEFINED behaviour or a crash if it does not get it. These load requests take priority over all others, and are
			//always filled, as its better to break loading proceedures than to break the game during runtime.
			unsigned int priority;
			RequestData() {};
			RequestData(FileType fType, std::string fName, unsigned int prio) : loadType(fType), fileName(fName), priority(prio) {};
		};
	private:
		RequestData request;
	public:
		RequestData* GetRequestData() { return &request; }
		LoadRequest() {};
		LoadRequest(RequestData data): request(data) {};
		LoadRequest(FileType fType, std::string fName, int prio) : request(RequestData(fType, fName, (unsigned int)prio)) {};


	};
	std::deque<LoadRequest> requestQueue;

	void* RetrieveData(FileType fType, std::string fileName, int prio) 
	{
		LoadRequest req = LoadRequest(fType, fileName, prio);
		LoadRequest::RequestData* reqData = req.GetRequestData();
		switch (reqData->loadType) 
		{
			case ImageFile: { 
				if (textureDataMap.find(reqData->fileName) != textureDataMap.end()) 
				{
					//WriteDebug("found some data: " + reqData->fileName);
					return &textureDataMap.find(reqData->fileName)->second;
				}
				else 
				{
					//WriteDebug("no data found -- loading " + fileName);
					LoadData(fType, reqData->fileName);
					return &textureDataMap.find(reqData->fileName)->second;
				}
				break; 
			}
			//case AudioFile: {
			//	if (&audioDataMap.find(reqData->fileName))
			//		WriteDebug("found some data");
			//	break;
			//}
			case LevelFile: {
				if (mapDataMap.find(reqData->fileName) != mapDataMap.end()) 
				{
					//WriteDebug("found some data: " + reqData->fileName);
					return &mapDataMap.find(reqData->fileName)->second;
				}
				else 
				{
					//WriteDebug("no data found -- loading " + fileName);
					LoadData(fType, reqData->fileName);
					return &mapDataMap.find(reqData->fileName)->second;
				}
				break;
			}
			case ObjFile: {
				if (modelDataMap.find(reqData->fileName) != modelDataMap.end()) 
				{
					WriteDebug("found some data -- " + fileName);
					return &modelDataMap.find(reqData->fileName)->second;
				}
				else 
				{
					WriteDebug("no data found -- loading " + fileName);
					LoadData(fType, reqData->fileName);
					return &modelDataMap.find(reqData->fileName)->second;
				}
				break;
			}
			case AnimFile: 
			{ 				
				if (animDataMap.find(reqData->fileName) != animDataMap.end())
				{
					WriteDebug("found some data -- " + fileName);
					return &animDataMap.find(reqData->fileName)->second;
				}
				else
				{
					WriteDebug("no data found -- loading " + fileName);
					LoadData(fType, reqData->fileName);
					return &animDataMap.find(reqData->fileName)->second;
				}
				break; 
			}
		}
		return nullptr;

		//requestQueue.push_back(LoadRequest(fType, data));
	}
private:

	//Loads data from data files based on the filetype and name of the file. Places the data inside LoadController Storage for later usage.
	void LoadData(FileType fType, std::string fileName) 
	{
		switch (fType)
		{
			case ImageFile: {
				int w, h, n;
				Texture newText = Texture(5, fileName, LoadImageFile(fType, fileName, w, h, n), w,h,n);
				textureDataMap.insert(std::pair<std::string, Texture>(fileName, newText));
				break;
			}
			//case AudioFile: {
			//	break;
			//}
			case LevelFile: {
				MapDataChunk newMap = LoadMapData(fileName);
				mapDataMap.insert(std::pair<std::string, MapDataChunk>(fileName, newMap));
				break;
			}
			case ObjFile: {

				ModelDataChunk newModel = Load3DModel(fType, fileName);
				modelDataMap.insert(std::pair<std::string, ModelDataChunk>(fileName, newModel));
				break;
			}
			case AnimFile: 
			{
				AnimData newAnim = LoadAnimData(fType, fileName);
				animDataMap.insert(std::pair<std::string, AnimData>(fileName, newAnim));
				break;
			}
		}

	};
	void UnloadData() 
	{
	}
};
LoadRequestController* GetLoadController();
