#include "shaders.h"

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