#version 330 core

//in vec4 fragInColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 fragOutColor;

void main()
{
	vec4 newTexture = texture(texture1, TexCoord) + texture(texture2, TexCoord);
	
    fragOutColor = newTexture;
}