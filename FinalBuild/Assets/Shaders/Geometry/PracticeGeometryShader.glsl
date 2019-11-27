#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2[] texCoord;
in vec3[] visVerts;
out vec3 outValue;
out vec2 TexCoord;

void main()
{
    for (int i = 0; i < 3; i++) {
        outValue = visVerts[0];
        EmitVertex();
    }

    EndPrimitive();
	TexCoord = texCoord[0];
}


