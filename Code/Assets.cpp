#include "assets.h"

//MUST be after all other #includes, and can only exist in 1 file. DO NOT MOVE
#pragma region
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

AnimData animDataPTR;
ModelDataChunk modelDataPTR;
unsigned char* imageData;


//Loads a shader by compiling a vertex shader, fragment shader and a geometry shader into a single Shader class.
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


//Loads an image. Requires image dimensions. Returns a pointer to a STBI memory location.
unsigned char* LoadImageFile(FileType fileType, std::string fileName, int& width, int& height, int& nrChannels)
{
	stbi_set_flip_vertically_on_load(true);
	std::string loadstr = FetchPath(ImageFile, fileName, false);
	imageData = stbi_load(loadstr.c_str(), &width, &height, &nrChannels, 0);
	if (!imageData)
		WriteDebug(stbi_failure_reason());

	return imageData;
}
//Frees up STBI image memory.
void UnloadImageFile(unsigned char* image)
{
	stbi_image_free(image);
}

//Loads a 3D model from a .wav file through reading text.
ModelDataChunk& Load3DModel(std::string fileName, FileType fileType)
{
	std::ifstream modelFile;
	std::string loadstr = FetchPath(fileType, fileName, false);
	modelFile.open(loadstr);
	if (!modelFile.is_open())
	{
		WriteDebug("ERROR -- Cannot Open File: " + fileName);
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
								//WriteDebug(textureName);
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
				vertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(0), glm::vec3(0)));
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
					else { WriteDebug("Cannot Load3dModel -- Too Many Face Vertexes. Triangulate Faces in Model Editor."); break; }
				}
			}
#pragma endregion
		}
#pragma endregion
	}
	else
		WriteDebug("Cannot Load3DModel -- Incorrect FileType");


	//WriteDebug(std::to_string(faces.size()));
	modelDataPTR = ModelDataChunk(vertices, indices, edges, textures/*, faces*/);

	//for (int i=0; i < vertices.size(); i++)
	//{
	//	WriteDebug(std::to_string(i+1) + ": " + std::to_string(vertices[i].position.x) + ", " + std::to_string(vertices[i].position.y) + ", " + std::to_string(vertices[i].position.z));
	//}
	//WriteDebug("Vertexes: " + std::to_string(vertices.size()));

	modelFile.close();
	return modelDataPTR;

}

//Loads an animation from a .txt file by reading line by line.
AnimData& LoadAnimData(std::string fileName)
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
	std::vector<TextureAnimFrame> frames;
	std::vector<std::vector<int>> textureDir;
	std::vector<AnimEvent> events;

	unsigned int length = 0;
	int i = 0;
	TextureAnimFrame freshFrame;

	for (std::string line = ""; std::getline(animFile, line);)
	{
		std::istringstream in(line);
		std::string type;
		in >> type;

		if (type == "t")
		{
			std::string Tword = "";
			int lineLength = 0;
			int wordNum = 0;

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
	for (int l = i; l < length; l++)
	{
		frames.push_back(freshFrame);
	}
	animDataPTR = AnimData(length, frames, textureLookups, events);
	animFile.close();
	return animDataPTR;
}