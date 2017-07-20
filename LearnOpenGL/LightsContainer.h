#pragma once

#ifndef LIGHTS_CONTAINER_H
#define LIGHTS_CONTAINER_H

#include "LightTypes.h"
#include <vector>
#include <string>
#include <fstream>

class LightsContainer
{
public:
	LightsContainer(GLchar* lightsLocation);
	~LightsContainer();

	DirLight * getDirLight();
	SpotLight * getSpotLight();
	std::vector<PointLight> * getPointLights();


private:
	GLboolean havePushedLastPointLight = false;
	void readLightingFile(GLchar* filename);

	/* Can be initialized at the top of the .cpp */
	static DirLight* dirLight;
	static SpotLight* spotLight;
	static std::vector<PointLight> pointLights;

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
};

#endif // !LIGHTS_CONTAINER_H
