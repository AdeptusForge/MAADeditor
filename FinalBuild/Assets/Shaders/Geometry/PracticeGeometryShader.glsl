#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3[] gVerts;
in vec3[] gColor;
in vec2[] gTexCoord;

out vec4 fVerts;
out vec3 fColor;
out vec2 fTexCoord;

void main()
{
    for (int i = 0; i < 3; i++) 
	{
		gl_Position = gl_in[i].gl_Position;
        fVerts = gl_in[i].gl_Position;
		fTexCoord = gTexCoord[i];
        EmitVertex();
    }
    EndPrimitive();
}


