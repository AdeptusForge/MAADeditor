#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 aTexCoord;

//out vec4 fragInColor;
out vec2 TexCoord;
out vec3 visVerts;

uniform mat4 transform;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	visVerts = vec3(17.0, 5.0, 0.0);
    gl_Position =  projection * view * model * vec4(aPos, 1.0);
	//fragInColor = vColor;
	TexCoord = aTexCoord;


}