#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

#include "LightTypes.h"

class Material {
public:

	Material();

	Material(GLchar* materialLocation, Shader shader);

private:
	struct MaterialProp {
		GLfloat shininess;
	};


	/* DirLight, SpotLight and PointLights were initialized in this->readLightingFile() */
	void sendDataToShader(Shader shader);

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
	void getMeshProperties(DirLight * dirLight, GLchar* context, GLchar lightPropType);

	void getMeshProperties(SpotLight * spotLight, GLchar* context, GLchar lightPropType);

	void getMeshProperties(PointLight * pointLight, GLchar* context, GLchar lightPropType);

	void getMeshProperties(MaterialProp * material, GLchar* context, GLchar lightPropType);

	DirLight dirLight;
	SpotLight spotLight;
	GLboolean havePushedLastPointLight = false;
	std::vector<PointLight> pointLights;
	MaterialProp material;
	void readLightingFile(GLchar* filename);
};

#endif // !MATERIAL_H