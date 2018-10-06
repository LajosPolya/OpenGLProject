#include "LightsContainer.h"


LightsContainer::LightsContainer(std::string lightsLocation) {
	this->propContainer.dirLight = new DirLight();
	this->propContainer.spotLight = new SpotLight();
	this->poitLightsTransform = new InstancedTransformImpl();
	readLightingFile(lightsLocation);
}

LightsContainer::~LightsContainer() {}

DirLight * LightsContainer::getDirLight() {
	return this->propContainer.dirLight;
}

SpotLight * LightsContainer::getSpotLight() {
	return this->propContainer.spotLight;
}

std::vector<PointLight> * LightsContainer::getPointLights() {
	return &this->propContainer.pointLights;
}

const DirLight * LightsContainer::getDirLight() const {
	return this->propContainer.dirLight;
}

const SpotLight * LightsContainer::getSpotLight() const {
	return this->propContainer.spotLight;
}

const std::vector<PointLight> * LightsContainer::getPointLights() const {
	return &this->propContainer.pointLights;
}

InstancedTransformImpl * LightsContainer::getPointLightTransform() {
	return this->poitLightsTransform;
}

void LightsContainer::addPointLight(glm::vec3 pos) {
	PointLight pointLight;
	pointLight.position = pos;
	pointLight.constant = (GLfloat)1.0;
	pointLight.linear = (GLfloat)0.09;
	pointLight.quadratic = (GLfloat)0.032;
	pointLight.ambient = glm::vec3((GLfloat)0.05);
	pointLight.diffuse = glm::vec3((GLfloat)0.8);
	pointLight.specular = glm::vec3((GLfloat)1.0);
	this->propContainer.pointLights.push_back(pointLight);
	this->poitLightsTransform->addNewInstance(pointLight.position, glm::vec3(0, 0, 0), glm::vec3(0.2, 0.2, 0.2));
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
void LightsContainer::getMeshProperties(DirLight * dirLight, GLchar * context, GLchar lightPropType) {
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
	else {
		std::cout << "ERROR::LightsContainer::Invalid DirLight Props" << std::endl;
	}
}

void LightsContainer::getMeshProperties(SpotLight * spotLight, GLchar * context, GLchar lightPropType) {
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
	}
	else if (lightPropType == '1') {
		spotLight->direction = prop;
	}
	else if (lightPropType == '2') {
		spotLight->ambient = prop;
	}
	else if (lightPropType == '3') {
		spotLight->diffuse = prop;
	}
	else if (lightPropType == '4') {
		spotLight->specular = prop;
	}
	else if (lightPropType == '5') {
		spotLight->constant = prop[0];
	}
	else if (lightPropType == '6') {
		spotLight->linear = prop[0];
	}
	else if (lightPropType == '7') {
		spotLight->quadratic = prop[0];
	}
	else if (lightPropType == '8') {
		spotLight->cutOff = prop[0];
	}
	else if (lightPropType == '9') {
		spotLight->outerCutOff = prop[0];
	}
	else {
		std::cout << "ERROR::LightsContainer::Invalid SpotLight Props" << std::endl;
	}
}

void LightsContainer::getMeshProperties(PointLight * pointLight, GLchar * context, GLchar lightPropType) {
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
	else {
		std::cout << "ERROR::LightsContainer::Invalid SpotLight Props" << std::endl;
	}
}

void LightsContainer::readLightingFile(std::string filename) {
	std::string line;
	std::ifstream file(filename);

	if (file.is_open()) {
		PointLight pointLight;
		GLchar prevPointLight = ' ';
		while (std::getline(file, line)) {
			GLchar * token;
			GLchar * context = NULL;

			token = strtok_s(&line[0], ",", &context);
			if (token[0] == '1') { // Directional Light
				getMeshProperties(this->propContainer.dirLight, context, token[1]);

				if (this->propContainer.pointLights.size() != 0 && this->havePushedLastPointLight == false) {
					this->propContainer.pointLights.push_back(pointLight);
					this->poitLightsTransform->addNewInstance(pointLight.position, { 0, 0, 0 }, { 0.2, 0.2, 0.2 });
					this->havePushedLastPointLight = true;
				}

			}
			else if (token[0] == '2') { // Point Light
				if (token[2] != prevPointLight) {
					if (prevPointLight != ' ' || this->propContainer.pointLights.size() != 0) {
						this->propContainer.pointLights.push_back(pointLight);
						this->poitLightsTransform->addNewInstance(pointLight.position, { 0, 0, 0 }, { 0.2, 0.2, 0.2 });
					}
					prevPointLight = token[2]; // nth Point Light instance
					pointLight = {};
				}
				getMeshProperties(&pointLight, context, token[1]);
			}
			else if (token[0] == '3') { // Spot Light
				getMeshProperties(this->propContainer.spotLight, context, token[1]);

				if (this->propContainer.pointLights.size() != 0 && this->havePushedLastPointLight == false) {
					this->propContainer.pointLights.push_back(pointLight);
					this->poitLightsTransform->addNewInstance(pointLight.position, { 0, 0, 0 }, { 0.2, 0.2, 0.2 });
					this->havePushedLastPointLight = true;
				}
			}
			else {
				std::cout << "ERROR::LightsContainer::Invalid Light Type" << std::endl;
			}
		}
	}
}
