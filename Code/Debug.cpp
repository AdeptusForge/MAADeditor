#include <string>
#include <fstream>
#include "Time.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma region glm vector to string
//Converts glm vectors into strings specifically for debugging purposes.
//--Overloads--
//glm::vec2/3/4
//glm::bvec2/3/4
std::string vecToStr(glm::vec2 vec) 
{
	std::string newStr = "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")";
	return newStr;
}
std::string vecToStr(glm::vec3 vec)
{
	std::string newStr = "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
	return newStr;
}
std::string vecToStr(glm::vec4 vec)
{
	std::string newStr = "(" +std::to_string(vec.w) + ", " + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
	return newStr;
}
std::string vecToStr(glm::bvec2 vec)
{
	std::string newStr = "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")";
	return newStr;
}std::string vecToStr(glm::bvec3 vec)
{
	std::string newStr = "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
	return newStr;
}
std::string vecToStr(glm::bvec4 vec)
{
	std::string newStr = "(" + std::to_string(vec.w) + ", " + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
	return newStr;
}

std::string quatToStr(glm::quat vec)
{
	std::string newStr = "(" + std::to_string(vec.w) + ", " + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
	return newStr;
}
#pragma endregion

//Writes a line of debug to the console.
//REFACTOR:: Allow Debug to show up during runtime outside of the console.
void WriteDebug(std::string phrase)
{
	std::cout <<  phrase << std::endl;
}
void WriteDebug(int phrase)
{
	std::cout << std::to_string(phrase) << std::endl;
}

//TODO:: Bug Reports.