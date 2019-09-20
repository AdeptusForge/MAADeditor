#pragma once
#include <ctime>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

bool TimeWorkingProperly();
void TimeStartup();
int GetCurrentTick();
void UpdateTime(GLFWwindow*);
