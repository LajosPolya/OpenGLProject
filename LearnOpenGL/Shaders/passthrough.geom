#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 NormalG [3];
in vec3 FragPosG [3];
in vec2 TexCoordsG [3];
in mat4 model[3];

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main() {
	gl_Position = gl_in[0].gl_Position;
	Normal = NormalG[0];
	FragPos = FragPosG[0];
	TexCoords = TexCoordsG[0];
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	Normal = NormalG[1];
	FragPos = FragPosG[1];
	TexCoords = TexCoordsG[1];
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	Normal = NormalG[2];
	FragPos = FragPosG[2];
	TexCoords = TexCoordsG[2];
	EmitVertex();
	EndPrimitive();
}