#pragma once


// TODO: Have a struct in this class which stores all properties of the class
// Position, Rotation, Scale, and Model
class InstancedTransform {
public:

	virtual std::vector<glm::vec3>* getPositions() = 0;

	virtual std::vector<glm::mat4> getModels() = 0;

	virtual void refreshModel() = 0;

	virtual void Draw() = 0;

protected:
	// This is protected so the derived classes have access to it
	struct InstancedTransformProps {
		std::vector<glm::vec3> Position;
		std::vector<glm::vec3> Rotation;
		std::vector<glm::vec3> Scale;

		std::vector<glm::mat4> model;
	};

private:

	virtual void readFile(GLchar* filename) = 0;
};