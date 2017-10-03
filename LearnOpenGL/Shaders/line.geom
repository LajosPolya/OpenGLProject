#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

in vec3 NormalG [3];
in vec3 FragPosG [3];
in vec2 TexCoordsG [3];

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main() {
	gl_Position = gl_in[0].gl_Position + vec3(0.0, 1.0, 0.0);
	Normal = NormalG[0];
	FragPos = FragPosG[0];
	TexCoords = TexCoordsG[0];
	EmitVertex();

	EndPrimitive();
}