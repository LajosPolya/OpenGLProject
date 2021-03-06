#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "LightTypes.h"


// TODO: HashMap to Access PointLights
/* 
 * Each PointLight should have an ID (which corresponds to its intdex in the vector<PointLight> pointLights;) 
 * This way a GameObject can access any light in the scene
 *
 * You can also have a HashMap who's key is the Position of the PointLight and the index
 * is the Entry of the PointLight thus a PointLight can be accessed by Position in O(2) time
 *
*/

struct MaterialProp {
	MaterialProp();
	MaterialProp(const MaterialProp & toCopy);
	GLfloat shininess;
};

class Material {
public:

	Material();
	Material(const Material & toCopy); // Copy Constructor

	Material(string materialLocation);
	Material(MaterialProp prop);

	GLfloat getShininess();
	MaterialProp getMaterialProp();

private:
	/* DirLight, SpotLight and PointLights were initialized in this->readLightingFile() in LightsContainer Class */
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
	void getMeshProperties(MaterialProp * material, GLchar * context, GLchar lightPropType);

	MaterialProp material;
	void readLightingFile(string filename);
};
