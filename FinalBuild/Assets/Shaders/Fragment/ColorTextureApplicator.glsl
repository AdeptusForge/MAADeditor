#version 330 core

//in vec4 fragInColor;
in vec2 TexCoord;


uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;


out vec4 fragOutColor;

void main()
{
	vec4 newTexture = texture(texture0, TexCoord);
	vec4 newTexture1 = texture(texture1, TexCoord);
	if(newTexture.a < 0.1)
        newTexture += newTexture1;


	fragOutColor = newTexture;
}