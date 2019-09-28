#version 330 core

in vec3 fragInColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

out vec4 fragOutColor;

void main()
{
    fragOutColor = texture(ourTexture, TexCoord);
}