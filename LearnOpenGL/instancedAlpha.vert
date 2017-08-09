#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 model[128];
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model[gl_InstanceID] * vec4(position, 1.0f);
	TexCoords = vec2(texCoords.x, 1.0f - texCoords.y);
}