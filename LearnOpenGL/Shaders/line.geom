#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in vec3 NormalG [1];
in vec3 FragPosG [1];
in vec2 TexCoordsG [1];
in mat4 model[1];

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = gl_in[0].gl_Position + (projection * model[0] * vec4(-0.25, 1.0, 0.0, 0.0));
	Normal = NormalG[0];
	FragPos = FragPosG[0];
	TexCoords = TexCoordsG[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + (projection * model[0] * vec4(0.0, 2.0, 0.0, 0.0));
	Normal = NormalG[0];
	FragPos = FragPosG[0];
	TexCoords = TexCoordsG[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + (projection * model[0] * vec4(0.25, 1.0, 0.0, 0.0));
	Normal = NormalG[0];
	FragPos = FragPosG[0];
	TexCoords = TexCoordsG[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + (projection * model[0] * vec4(0.0, 0.0, 0.0, 0.0));
	Normal = NormalG[0];
	FragPos = FragPosG[0];
	TexCoords = TexCoordsG[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + (projection * model[0] * vec4(-0.25, 1.0, 0.0, 0.0));
	Normal = NormalG[0];
	FragPos = FragPosG[0];
	TexCoords = TexCoordsG[0];
	EmitVertex();

	EndPrimitive();
}