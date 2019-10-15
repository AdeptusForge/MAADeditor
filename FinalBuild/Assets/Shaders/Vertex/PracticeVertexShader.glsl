#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 aTexCoord;

//out vec4 fragInColor;
out vec2 TexCoord;

uniform mat4 transform;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    gl_Position = model * view * vec4(aPos, 1.0);
	//fragInColor = vColor;
	TexCoord = aTexCoord;


}