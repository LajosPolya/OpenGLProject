#include "LightsContainer.h"


/* Static vars must be rediefined in .cpp (they can also be initialized here) */
DirLight* LightsContainer::dirLight = NULL;
SpotLight*  LightsContainer::spotLight = NULL;
std::vector<PointLight> LightsContainer::pointLights;


LightsContainer::LightsContainer(GLchar* lightsLocation)
{	
	if (lightsLocation != NULL) {
		/* Should only read Scene Lights once */
		/* This can also be implemented such that is saves the file name
		 * and if it's already been read in then it won't be read again */
		if (dirLight == NULL && spotLight == NULL && pointLights.size() == 0) {
			std::cout << "this is STATIC" << std::endl;
			dirLight = new DirLight();
			spotLight = new SpotLight();
			this->readLightingFile(lightsLocation);
		}
	}
}


LightsContainer::~LightsContainer()
{
}


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
void LightsContainer::getMeshProperties(DirLight * dirLight, GLchar* context, GLchar lightPropType) {
	GLchar * token;
	GLuint i = 0;
	glm::vec3 prop;

	token = strtok_s(NULL, ",", &context);
	while (token != NULL) {
		prop[i] = std::stof(token, NULL);
		token = strtok_s(NULL, ",", &context);
		i++;
	}

	if (lightPropType == '1') {
		dirLight->direction = prop;
	}
	else if (lightPropType == '2') {
		dirLight->ambient = prop;
	}
	else if (lightPropType == '3') {
		dirLight->diffuse = prop;
	}
	else if (lightPropType == '4') {
		dirLight->specular = prop;
	}

	///std::cout << prop.x << ", " << prop.y << ", " << prop.z << std::endl;
}

void LightsContainer::getMeshProperties(SpotLight * spotLight, GLchar* context, GLchar lightPropType) {
	GLchar * token;
	GLuint i = 0;
	glm::vec3 prop;

	token = strtok_s(NULL, ",", &context);
	while (token != NULL) {
		prop[i] = std::stof(token, NULL);
		token = strtok_s(NULL, ",", &context);
		i++;
	}

	if (lightPropType == '0') {
		spotLight->position = prop;
		///std::cout << prop.x << ", " << prop.y << ", " << prop.z << std::endl;
	}
	else if (lightPropType == '1') {
		spotLight->direction = prop;
		///std::cout << prop.x << ", " << prop.y << ", " << prop.z << std::endl;
	}
	else if (lightPropType == '2') {
		spotLight->ambient = prop;
		///std::cout << prop.x << ", " << prop.y << ", " << prop.z << std::endl;
	}
	else if (lightPropType == '3') {
		spotLight->diffuse = prop;
		///std::cout << prop.x << ", " << prop.y << ", " << prop.z << std::endl;
	}
	else if (lightPropType == '4') {
		spotLight->specular = prop;
		///std::cout << prop.x << ", " << prop.y << ", " << prop.z << std::endl;
	}
	else if (lightPropType == '5') {
		spotLight->constant = prop[0];
		///std::cout << prop.x << std::endl;
	}
	else if (lightPropType == '6') {
		spotLight->linear = prop[0];
		///std::cout << prop.x << std::endl;
	}
	else if (lightPropType == '7') {
		spotLight->quadratic = prop[0];
		///std::cout << prop.x << std::endl;
	}
	else if (lightPropType == '8') {
		spotLight->cutOff = prop[0];
		///std::cout << prop.x << std::endl;
	}
	else if (lightPropType == '9') {
		spotLight->outerCutOff = prop[0];
		///std::cout << prop.x << std::endl;
	}
}

void LightsContainer::getMeshProperties(PointLight * pointLight, GLchar* context, GLchar lightPropType) {
	GLchar * token;
	GLuint i = 0;
	glm::vec3 prop;

	token = strtok_s(NULL, ",", &context);
	while (token != NULL) {
		prop[i] = std::stof(token, NULL);
		token = strtok_s(NULL, ",", &context);
		i++;
	}

	if (lightPropType == '0') {
		pointLight->position = prop;
	}
	else if (lightPropType == '2') {
		pointLight->ambient = prop;
	}
	else if (lightPropType == '3') {
		pointLight->diffuse = prop;
	}
	else if (lightPropType == '4') {
		pointLight->specular = prop;
	}
	else if (lightPropType == '5') {
		pointLight->constant = prop[0];
	}
	else if (lightPropType == '6') {
		pointLight->linear = prop[0];
	}
	else if (lightPropType == '7') {
		pointLight->quadratic = prop[0];
	}
}

void LightsContainer::readLightingFile(GLchar* filename) {
	std::string line;
	std::ifstream file(filename);
	GLint vertexProp_BitMap = 0;
	if (file.is_open()) {
		PointLight pointLight;
		GLchar prevPointLight = ' ';
		while (std::getline(file, line)) {
			GLchar * token;
			GLchar* context = NULL;
			//std::cout << line << std::endl;

			token = strtok_s(&line[0], ",", &context);
			if (token[0] == '1') { // Directional Light
				getMeshProperties(this->dirLight, context, token[1]);

				if (pointLights.size() != 0 && this->havePushedLastPointLight == false) {
					pointLights.push_back(pointLight);
					this->havePushedLastPointLight = true;
				}

			}
			else if (token[0] == '2') { // Point Light
				if (token[2] != prevPointLight) {
					if (prevPointLight != ' ' || pointLights.size() != 0) {
						pointLights.push_back(pointLight);
					}
					prevPointLight = token[2];
					pointLight = {};
				}
				getMeshProperties(&pointLight, context, token[1]);
			}
			else if (token[0] == '3') { // Spot Light
				getMeshProperties(this->spotLight, context, token[1]);

				if (pointLights.size() != 0 && this->havePushedLastPointLight == false) {
					pointLights.push_back(pointLight);
					this->havePushedLastPointLight = true;
				}
			}
		}
	}
}
