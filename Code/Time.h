#pragma once
#include <ctime>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

void TimeStartup();
int GetCurrentTick();
void UpdateTime(GLFWwindow*);
