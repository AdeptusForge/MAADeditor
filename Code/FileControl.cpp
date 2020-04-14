#include "framework.h"
#include <fstream>
#include "Debug.h"
#include "algorithm"
#include "FileControl.h"
#include "SDL.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "SDL_mixer.h"
#include "Shaders.h"
#include <sstream>
#include <commdlg.h>
#include <fstream>
#include "Map.h"
#include <typeinfo>
#include "memory"

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

		case LevelFile:
		{
			path += "Levels/";
			fileName += ".map";
			break;
		}
		case AnimFile:
		{
			path += "Animations/";
			fileName += ".anim";
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
		case ObjTextureFile:
		{
			path += "Models/";
			fileName += ".txt";
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
	unsigned char* imageData = stbi_load(loadstr.c_str(), &width, &height, &nrChannels, 0);
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

AnimData LoadAnimData(std::string fileName) 
{
	std::ifstream animFile;
	std::string loadstr = FetchPath(AnimFile, fileName, false);
	animFile.open(loadstr);
	if (!animFile.is_open())
	{
		WriteDebug("Cannot Open File: " + fileName);
	}
	else
		WriteDebug("Loading File..." + fileName);

	std::vector<Texture> textureLookups;
	std::vector<AnimFrame> frames;
	std::vector<std::vector<int>> textureDir;
	std::vector<AnimEvent> events;

	unsigned int length =0;
	int i = 0;
	AnimFrame freshFrame;

	for (std::string line = ""; std::getline(animFile, line);)
	{
		std::istringstream in(line);
		std::string type;
		in >> type;
		
		if (type == "t") 
		{
			std::string Tword = "";
			int lineLength =0;
			int wordNum=0;

			for (auto x : line)
			{

				lineLength++;
				if (x != ' ')
					Tword = Tword + x;
				else if (x == ' ' && Tword.size() > 0)
				{
					if (Tword != "t") 
					{
						//WriteDebug(Tword);
						textureLookups.push_back(Texture(0, Tword));
						wordNum++;
					}
					Tword = "";
				}
				if (lineLength >= line.size() && Tword.size() > 1)
				{
					//WriteDebug(Tword);
					textureLookups.push_back(Texture(0, Tword));
					wordNum++;
					Tword = "";

				}
			}
		};
		if (type == "l") { in >> length; } 
		if (type == "f") 
		{

			i++;
			std::string Fword = "";
			int lineLength = 0;
			for (auto x : line)
			{
				lineLength++;
				if (x != ' ') 
				{
					Fword = Fword + x;
				}
				if (x == ' ' || lineLength >= line.size()) 
				{
					if (Fword.size() > 1) 
					{
						
						std::istringstream buf(Fword);
						int textureNumber, textureLookup;
						char c1, c2, c3;
						std::string func, var1, var2;
						if (buf >> textureNumber >> c1 >> textureLookup && c1 == '/')
						{
							freshFrame.SetTextureChange(textureNumber, textureLookup);
						}
						//AnimEvent Creation

					}
					Fword = "";
				}
			}
			frames.push_back(freshFrame);
		}
#pragma region AnimEventLoading
		if (type == "u") 
		{
			//std::string eventData;
			//in >> eventData;
			//WriteDebug(eventData);
			//std::string functionName, var1, var2;
			//char c1, c2, c3;
			//int f;

			//std::string Fword = "";
			//int lineLength = 0;
			//for (auto x : eventData) 
			//{
			//	lineLength++;
			//	if (x != ' ')
			//	{
			//		Fword = Fword + x;
			//	}
			//	if (x == ' ' || lineLength >= line.size())
			//	{
			//		if (Fword.size() > 1)
			//		{
			//		}
			//	}
			//}
		}
#pragma endregion

	}
	for (int l =i;l < length; l++)
	{
		frames.push_back(freshFrame);
	}
	AnimData freshAnim = AnimData(length, frames, textureLookups, events);

	return freshAnim;
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

	if (fileType == ObjFile)
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

#pragma region Load Textures

			if (type == "mtllib")
			{
				std::string Tstr;
				in >> Tstr;
				std::ifstream textureFile;
				std::string loadTstr = FetchPath(ObjTextureFile, Tstr, false);
				textureFile.open(loadTstr);
				if (!textureFile.is_open())
				{
					WriteDebug("Cannot Open File: " + loadTstr);
				}
				else
					WriteDebug("Loading File..." + loadTstr);
				for (std::string Tline; std::getline(textureFile, Tline);) 
				{

					std::istringstream texturein(Tline);
					int i = 0;

					texturein >> Tstr;
					if (Tstr == "newmtl") 
					{
						if (i >= MAX_MODEL_TEXTURES) { WriteDebug("Too many textures in file" + fileName); break; }
						else
						{
							if (Tstr.size() > 0)
							{
								std::string textureName;
								texturein >> textureName;
								WriteDebug(textureName);
								i++;
								textures.push_back(Texture(0, textureName));
							}
						}
					}
				}
			}
#pragma endregion

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
							//WriteDebug(word + " : full word");
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
							//WriteDebug(word + " : full word");
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
							//WriteDebug("End of Line");
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
	ModelDataChunk newModel = ModelDataChunk(vertices, indices, edges, textures/*, faces*/);

	//for (int i=0; i < vertices.size(); i++)
	//{
	//	WriteDebug(std::to_string(i+1) + ": " + std::to_string(vertices[i].position.x) + ", " + std::to_string(vertices[i].position.y) + ", " + std::to_string(vertices[i].position.z));
	//}
	//WriteDebug("Vertexes: " + std::to_string(vertices.size()));

	modelFile.close();
	return newModel;

}

MapDataChunk LoadMapData(std::string fileName)
{
	std::ifstream mapFile;

	std::string loadstr = FetchPath(LevelFile, fileName, false);
	mapFile.open(loadstr);
	if (!mapFile.is_open())
	{
		WriteDebug("Cannot Open File: " + fileName);
	}
	else
		WriteDebug("Loading File..." + fileName);
	unsigned int x, y;
	std::vector<MapTile> tiles;

	for (std::string line; std::getline(mapFile, line);)
	{
		std::istringstream in(line);
		std::string type;
		in >> type;

		int n, s, e, w, orient;
		std::string tileModel;
		std::vector<std::string> functionTriggers;

		if (type == "s") 
		{ 
			int testX, testY;
			in >> testX >> testY;
			if (testX <= 0 || testY <= 0)
				WriteDebug("File Error: Map Size incorrect");
			else { x = testX; y = testY; }
		}
		if (type == "t")
		{
			in >> n >> s >> e >> w >> tileModel >> orient;
		}
		if (type == "f")
		{
			std::string functionsLine = line;
			int functionCount = std::count(functionsLine.begin(), functionsLine.end(), ' ');
			for (int i = 0; i < functionCount; i++) 
			{
				std::string freshFunction;
				in >> freshFunction;
				functionTriggers.push_back(freshFunction);
			}
			tiles.push_back(MapTile(glm::ivec2(x,y), 
				(TileFeature)n, (TileFeature)s, (TileFeature)e, (TileFeature)w, 
				tileModel, (MapDirection)orient, functionTriggers));
		}
	}

	MapDataChunk freshMapData = MapDataChunk(x,y, tiles);
	mapFile.close();
	return freshMapData;
};

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