#version 330 core

struct Material {
	sampler2D diffuse;
};

in vec2 TexCoords;

out vec4 color;

uniform Material material;

void main()
{
	color = texture(material.diffuse, TexCoords);
}