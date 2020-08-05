#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 aTexCoord;

//out vec4 fragInColor;
out vec2 gTexCoord;
out vec3 gVerts;
out vec3 gColor;

uniform mat4 transform;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform vec3 cameraPos;
uniform vec3 cameraFront;
vec3 newFront = cameraFront;

void main()
{
	gVerts = aPos;
    gl_Position =  projection * view * model * vec4(aPos + vec3(0,0,-5), 1.0);
	gColor = color;
	gTexCoord = aTexCoord;
}