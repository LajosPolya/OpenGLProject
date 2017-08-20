#include "TransparentTransformImpl.h"
#include "TransparentGameObjectImpl.h"

TransparentTransformImpl::TransparentTransformImpl() {}

TransparentTransformImpl::~TransparentTransformImpl() {}

TransparentTransformImpl::TransparentTransformImpl(GLchar* fileLocation, TransparentGameObjectImpl * gameObject) {
	this->gameObject = gameObject;
	this->readFile(fileLocation);
}

glm::vec3* TransparentTransformImpl::getPositions(GLuint i)
{
	return &this->props[i].Position;
}

glm::mat4 TransparentTransformImpl::getModel(GLuint i)
{
	return this->props[i].model;
}

std::vector<glm::mat4> TransparentTransformImpl::getModels()
{
	return this->models;
}

void TransparentTransformImpl::refreshModel() {
	std::sort(this->props.begin(), this->props.end(), [this](InstancedTransformProps const &x, InstancedTransformProps const& y) { return this->sortFunction(x, y); });
}

void TransparentTransformImpl::Draw() {
	/* This resets to the original values from file */
	// this->newRotation = this->Rotation;
	// this->newPosition = this->Position;
	// this->newScale = this->Scale;
}

TransparentGameObjectImpl * TransparentTransformImpl::getGameObject()
{
	return this->gameObject;
}

GLuint TransparentTransformImpl::getSize()
{
	return this->props.size();
}

// TODO: Create a Base Class which will have this method (implemented by this and TransparentTransformImpl)
void TransparentTransformImpl::readFile(GLchar * filename)
{
	std::ifstream file(filename);
	GLint vertexProp_BitMap = 0;
	if (file.is_open()) {
		std::string line;

		while (std::getline(file, line)) {
			// std::cout << line << std::endl;
			GLchar * token;
			GLchar* context = NULL;
			GLuint i = 0;
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
			glm::mat4 model;
			InstancedTransformProps prop;

			token = strtok_s(&line[0], ",", &context);
			position.x = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			position.y = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			position.z = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			rotation.x = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			rotation.y = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			rotation.z = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			scale.x = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			scale.y = std::stof(token);

			token = strtok_s(NULL, ",", &context);
			scale.z = std::stof(token);

			model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::translate(model, position);

			model = glm::scale(model, scale);

			prop.Position = position;
			prop.Rotation = rotation;
			prop.Scale = scale;
			prop.model = model;
			this->props.push_back(prop);

			this->models.push_back(this->props[this->props.size()-1].model);
		}
	}
	else {
		std::cout << "ERROR::BAD TRANSFORM FILENAME" << std::endl;
	}
}


/* Note this sort returns the opposite of what a sort function is supposed to */
GLboolean TransparentTransformImpl::sortFunction(InstancedTransformProps const &x, InstancedTransformProps const &y)
{
	GLfloat dist1 = glm::length(this->gameObject->getCamera()->Position - x.Position);
	GLfloat dist2 = glm::length(this->gameObject->getCamera()->Position - y.Position);
	return dist1 > dist2;
}
