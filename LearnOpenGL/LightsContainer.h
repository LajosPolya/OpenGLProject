#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "LightTypes.h"
#include "InstancedTransformImpl.h"

using namespace std;
using namespace glm;


class LightsContainer
{
public:
	LightsContainer(string lightsLocation);
	~LightsContainer();

	DirLight * getDirLight();
	SpotLight * getSpotLight();
	vector<PointLight> * getPointLights();

	const DirLight * getDirLight() const;
	const SpotLight * getSpotLight() const;
	const vector<PointLight> * getPointLights() const;

	InstancedTransformImpl * getPointLightTransform();

	void addPointLight(vec3 pos);

private:
	GLboolean havePushedLastPointLight = false;
	void readLightingFile(string filename);

	LightsPropertiesContainer propContainer;

	InstancedTransformImpl * poitLightsTransform = nullptr;

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
	void getMeshProperties(DirLight * dirLight, GLchar * context, GLchar lightPropType);
	void getMeshProperties(SpotLight * spotLight, GLchar * context, GLchar lightPropType);
	void getMeshProperties(PointLight * pointLight, GLchar * context, GLchar lightPropType);
};
