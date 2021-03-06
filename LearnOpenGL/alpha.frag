#version 330 core

struct Material {
	sampler2D diffuse;
};

in vec2 TexCoords;

out vec4 color;

uniform Material material;

void main()
{
	vec4 texColor = texture(material.diffuse, TexCoords);
	if(texColor.a < 0.1) {
		discard;
	}
	color = texColor;
}