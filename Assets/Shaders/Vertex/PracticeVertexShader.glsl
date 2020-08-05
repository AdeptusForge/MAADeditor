#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 aUV;

//out vec4 fragInColor;
out vec2 gTexCoord;
out vec3 gVerts;
out vec3 gColor;


uniform int pColumns;
uniform int pRows;
uniform sampler2D sceneTex;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gVerts = aPos;
    gl_Position =  projection * view * model * vec4(aPos, 1.0);
	gColor = color;

	float pX = aUV.x;
	float pY = aUV.y;





	vec2 pixelatedUV = vec2(pX,pY);
	gTexCoord = pixelatedUV; 
}