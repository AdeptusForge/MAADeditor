#include <string>
#include <fstream>
#include "Time.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

std::string quatToStr(glm::quat vec)
{
	std::string newStr = "(" + std::to_string(vec.w) + ", " + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
	return newStr;
}


void WriteDebug(std::string phrase)
{
	std::cout <<  phrase << std::endl;
}
void WriteDebug(int phrase)
{
	std::cout << std::to_string(phrase) << std::endl;
}