#include "Material.h"

Material::Material() {}

Material::Material(const Material & toCopy) {
	this->material = toCopy.material;
}

Material::Material(string materialLocation) {

	this->readLightingFile(materialLocation);
}

Material::Material(MaterialProp prop) {
	this->material = prop;
}

GLfloat Material::getShininess() {
	if (this != nullptr) {
		return material.shininess;
	}
	return (GLfloat)0.0f;
}

MaterialProp Material::getMaterialProp() {
	return this->material;
}

/* DirLight, SpotLight and PointLights were initialized in this->readLightingFile() */
/*
Light Prop Type {
0 : Position      (PointLight, maybe SpotLight in the future)
1 : Direction     (DirLight, maybe SpotLight in the future)
2 : Ambient       (DirLight, PointLight, SpotLight)
3 : Diffuse       (DirLight, PointLight, SpotLight)
4 : Specular      (DirLight, PointLight, SpotLight)
5 : Constant      (PointLight, SpotLight)
6 : Linear        (PointLight, SpotLight)
7 : Quadratic     (PointLight, SpotLight)
8 : Cut Off       (SpotLight)
9 : Outer Cut Off (SpotLight)
}
Read Doc/Material.txt for more info
*/
void Material::getMeshProperties(MaterialProp * material, GLchar * context, GLchar lightPropType) {
	GLchar * token;
	GLuint i = 0;
	vec3 prop;

	token = strtok_s(NULL, ",", &context);
	while (token != NULL) {
		prop[i] = stof(token, NULL);
		token = strtok_s(NULL, ",", &context);
		i++;
	}

	/* MaterialProp only contains Shininess
	Shininess may be refered to as "Constant" for PointLight
	*/
	if (lightPropType == '5') {
		material->shininess = prop[0];
	}
}

void Material::readLightingFile(string filename) {
	string line;
	ifstream file(filename);
	if (file.is_open()) {
		PointLight pointLight;
		while (getline(file, line)) {
			GLchar * token;
			GLchar * context = NULL;

			// TODO: The Material file is still the Lighting file but only one field is used
			token = strtok_s(&line[0], ",", &context);
			if (token[0] == '4') {
				getMeshProperties(&this->material, context, token[1]);
			}
		}
	}
}

MaterialProp::MaterialProp() {}

MaterialProp::MaterialProp(const MaterialProp & toCopy) {
	this->shininess = toCopy.shininess;
}
