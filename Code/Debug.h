#pragma once
#include "framework.h"
#include <fstream>
#include <String>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


std::string vecToStr(glm::vec2 vec);
std::string vecToStr(glm::vec3 vec);
std::string vecToStr(glm::vec4 vec);
std::string quatToStr(glm::quat vec);

void WriteDebug(std::string phrase);
void WriteDebug(int phrase);


