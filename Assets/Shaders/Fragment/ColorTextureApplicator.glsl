#version 330 core

//in vec4 fragInColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 fragOutColor;

void main()
{
	//Remove Texture Alpha
//	vec4 texColor = texture(texture1, TexCoord);
//    if(texColor.a < 0.1)
//        discard;
//	texColor = texture(texture2, TexCoord);
//	if(texColor.a < 0.1)
//        discard;

    fragOutColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
}