#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layour (location = 3) in mat4 instanceModel;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * instanceModel * vec4(position, 1.0f);
	// This should be calculated bt the CPU and sent of as a uniform because currently this will get executed on every vertex
	Normal = mat3(transpose(inverse(instanceModel))) * normal; // Creates the Normal Matrix which allows non-uniform scaling on objects
	FragPos = vec3(instanceModel * vec4(position, 1.0f)); // Do all lighting calulations in World Space
	TexCoords = texCoords;
}