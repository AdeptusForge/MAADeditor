#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include <stdio.h>

void EditorStartup(GLFWwindow*);
void EditorUpdate(GLFWwindow*);
void EditorShutdown();

void FetchVisibleVerts();