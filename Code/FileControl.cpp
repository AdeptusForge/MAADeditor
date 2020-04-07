#include "framework.h"
#include <fstream>
#include "Debug.h"
#include "FileControl.h"
#include "SDL.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "SDL_mixer.h"
#include "Shaders.h"
#include <sstream>
#include <commdlg.h>
#include <fstream>

//MUST be after all other #includes, and can only exist in 1 file. DO NOT MOVE
#pragma region
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#include <direct.h>
#define GetCurrentDir _getcwd
std::string GetCurrentWorkingDir(void) {
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}
#pragma endregion


const unsigned int MAX_MODEL_TEXTURES = 8;
unsigned char* imageData;
ModelDataChunk newModel;


//The baseline file path that leads to all saved/loaded files
const std::string assetPath = GetCurrentWorkingDir();

bool VerifyFileOrFolder(std::string filePath) 
{
	filePath = "/" + filePath;
	if (filePath != assetPath)
		filePath = assetPath + filePath;
	DWORD ftyp = GetFileAttributesA(filePath.c_str());
	if (ftyp != INVALID_FILE_ATTRIBUTES) 
	{
		//WriteDebug("File found: " + filePath);
		return true;
	}
	//WriteDebug("File not found: " + filePath);
	return false;
}

void FileControlStartup()
{
	VerifyFileOrFolder(assetPath);
	WriteDebug("File Control Startup -- Successful");

}

std::string FetchPath(FileType fileType, std::string fileName, bool saving)
{
	std::string path;
	path += "Assets/";

	switch (fileType)
	{
		case None:
		{
			break;
		}
		case ImageFile:
		{
			path += "Textures/";
			fileName += ".png";
			break;
		}
		case AudioFile:
		{
			path += "Sounds/";
			fileName += ".wav";

			break;
		}
		case ModelFile:
		{
			path += "Models/";
			fileName += ".txt";
			break;
		}
		case LevelFile:
		{
			path += "Levels/";
			fileName += ".png";
			break;
		}
		case AnimFile:
		{
			path += "Animations/";
			fileName += ".png";
			break;
		}
		case PrefabFile:
		{
			path += "Prefabs/";
			fileName += ".png";
			break;
		}
		case SaveFile:
		{
			path += "Saves/";
			fileName += ".png";
			break;
		}

		case VertexShaderFile:
		{
			path += "Shaders/Vertex/";
			fileName += ".glsl";
			break;
		}
		case FragmentShaderFile:
		{
			path += "Shaders/Fragment/";
			fileName += ".glsl";
			break;
		}

		case GeometryShaderFile:
		{
			path += "Shaders/Geometry/";
			fileName += ".glsl";
			break;
		}

		case ObjFile:
		{
			path += "Models/";
			fileName += ".obj";
			break;
		}
	}
	path += fileName;
	VerifyFileOrFolder(path);

	return path;
}


void SaveActiveFile(FileType fileType, std::string fileName, std::string data)
{
	std::string filePath = FetchPath(fileType, fileName, true);
	WriteDebug(filePath);
	std::ofstream file;
	file.open(filePath + ".txt");
	file << data;
	file.close();
}


unsigned char* LoadImageFile(FileType fileType, std::string fileName, int &width, int &height, int &nrChannels)
{
	stbi_set_flip_vertically_on_load(true);
	std::string loadstr = FetchPath(ImageFile, fileName, false);
	imageData = stbi_load(loadstr.c_str(), &width, &height, &nrChannels, 0);
	if(!imageData)
		WriteDebug(stbi_failure_reason());
	return imageData;
}

Mix_Chunk* LoadGameAudioFile(std::string fileName)
{
	std::string loadstr = FetchPath(AudioFile, fileName, false);

	Mix_Chunk *sample = Mix_LoadWAV(loadstr.c_str());
	if (sample == NULL) WriteDebug("Sound not found: " + std::string(loadstr));

	return sample;
}

ModelDataChunk Load3DModel(std::string fileName, FileType fileType)
{
	std::ifstream modelFile;
	std::string loadstr = FetchPath(fileType, fileName, false);
	modelFile.open(loadstr);
	if (!modelFile.is_open())
	{
		WriteDebug("Cannot Open File: " + fileName);
	}
	else
		WriteDebug("Loading File..." + fileName);

	std::vector<Texture> textures;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Edge> edges;
	std::vector<std::pair <int, int>> faces;

	if (fileType == ModelFile)
	{
#pragma region Read MAADFile

		int it = 0;
		int amt = 0;
		for (std::string line; std::getline(modelFile, line);)
		{
#pragma region Load Textures
			std::istringstream in(line);
			std::string type;
			in >> type;
			int i = 0;
			if (type == "t")
			{
				if (i >= MAX_MODEL_TEXTURES) { WriteDebug("Too many textures in file" + fileName); break; }
				else
				{
					i++;
					std::string str;
					in >> str;
					if (str.size() > 0)
					{
						textures.push_back(Texture(0, str));
					}
				}
			}
#pragma endregion

#pragma region  Load Indices & Generate Faces

			if (type == "f")
			{
				if (amt > 0)
				{
					faces.push_back(std::make_pair(it, amt));
					it += amt;
					amt = 0;
				}
			}

			if (type == "i")
			{
				amt++;
				int tri1, tri2, tri3;
				in >> tri1 >> tri2 >> tri3;
				indices.push_back(tri1);
				indices.push_back(tri2);
				indices.push_back(tri3);
			}
#pragma endregion

#pragma region Load Vertices
			if (type == "v")
			{
				float x, y, z,			//position
					x2, y2, z2,			//color
					x3, y3;			//texCoord
				in >> x >> y >> z >>
					x2 >> y2 >> z2 >>
					x3 >> y3;
				vertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(x2, y2, z2), glm::vec2(x3, y3)));

			}
#pragma endregion

#pragma region Load Edges
			if (type == "e")
			{
				float x, y, z,			//start
					x2, y2, z2,			//end
					x3, y3, z3;			//color
				in >> x >> y >> z >>
					x2 >> y2 >> z2 >>
					x3 >> y3 >> z3;
				edges.push_back(Edge(glm::vec3(x, y, z), glm::vec3(x2, y2, z2), glm::vec3(x3, y3, z3)));
			}

#pragma endregion
		}
#pragma endregion
	}
	else if (fileType == ObjFile)
	{
#pragma region Read ObjFile

		std::vector<glm::vec3> vertCoords;
		std::vector<glm::vec3> vertTexCoords;
		std::vector<glm::vec3> vertNorms;


		for (std::string line; std::getline(modelFile, line);)
		{
			std::istringstream in(line);

			std::string type;
			in >> type;
#pragma region Vertex Loading
			if (type == "v")
			{
				float x, y, z;
				in >> x >> y >> z;
				vertices.push_back(Vertex(glm::vec3(x,y,z),glm::vec3(0), glm::vec3(0)));
			}
			if (type == "vt")
			{
				float x, y, z;
				in >> x >> y >> z;
				vertTexCoords.push_back(glm::vec3(x, y, z));
			}
			if (type == "vn")
			{
				float x, y, z;
				in >> x >> y >> z;
				vertNorms.push_back(glm::vec3(x, y, z));
			}
#pragma endregion

#pragma region Indice Loading & Vertex Assignment
			if (type == "f") 
			{
				std::string word = "";
				int vertLimit = 0;
				int length = 0;
				for (auto x : line)
				{
					length++;
					if (vertLimit < 3)
					{
						if (x != 'f' && x != ' ')
							word = word + x;
						if (x == ' ' && word.size() > 0) 
						{
							std::istringstream buf(word);
							WriteDebug(word + " : full word");
							int v, vt, vn;
							char c1;

							if (buf >> v >> c1 >> vt && c1 == '/') /*   f #/# format  */
							{
								vertices[v - 1].texCoords = vertTexCoords[vt - 1];
							}
							else if (buf >> v >> c1 >> vt >> c1 >> vn && c1 == '/') /*   f #/#/# format  */
							{
								vertices[v - 1].texCoords = vertTexCoords[vt - 1];
							}
							else if (buf >> v >> c1 >> c1 >> vn && c1 == '/') /*   f #//# format  */
							{
								vertices[v - 1].texCoords = vertTexCoords[vt - 1];
							}
							indices.push_back(v - 1);
							vertLimit++;
							word = "";
						}
						if (length >= line.size()) 
						{
							std::istringstream buf(word);
							WriteDebug(word + " : full word");
							int v = 0, vt, vn;
							char c1;

							if (buf >> v >> c1 >> vt && c1 == '/') /*   f #/# format  */
							{
								vertices[v - 1].texCoords = vertTexCoords[vt - 1];
							}
							else if (buf >> v >> c1 >> vt >> c1 >> vn && c1 == '/') /*   f #/#/# format  */
							{
								vertices[v - 1].texCoords = vertTexCoords[vt - 1];
							}
							else if (buf >> v >> c1 >> c1 >> vn && c1 == '/') /*   f #//# format  */
							{
								vertices[v - 1].texCoords = vertTexCoords[vt - 1];
							}
							indices.push_back(v - 1);
							vertLimit++;
							WriteDebug("End of Line");
						}
					}
					else { WriteDebug("Cannot Load3dModel -- Too Many Face Vertexes"); break; }
				}
			}
#pragma endregion
		}
#pragma endregion
	}
	else
		WriteDebug("Cannot Load3DModel -- Incorrect FileType"); 
	

	//WriteDebug(std::to_string(faces.size()));
	newModel = ModelDataChunk(vertices, indices, edges, textures/*, faces*/);

	//for (int i=0; i < vertices.size(); i++)
	//{
	//	WriteDebug(std::to_string(i+1) + ": " + std::to_string(vertices[i].position.x) + ", " + std::to_string(vertices[i].position.y) + ", " + std::to_string(vertices[i].position.z));
	//}
	//WriteDebug("Vertexes: " + std::to_string(vertices.size()));

	modelFile.close();
	return newModel;

}

Shader LoadCustomShader(std::string vertexPath, std::string fragmentPath, std::string geometryPath)
{
	vertexPath = FetchPath(VertexShaderFile, vertexPath, false);
	fragmentPath = FetchPath(FragmentShaderFile, fragmentPath, false);
	geometryPath = FetchPath(GeometryShaderFile, geometryPath, false);

	const GLchar* newVertexPath = vertexPath.c_str();
	const GLchar* newFragmentPath = fragmentPath.c_str();
	const GLchar* newGeometryPath = geometryPath.c_str();

	Shader ourShader(newVertexPath, newFragmentPath, newGeometryPath);
	return ourShader;
}