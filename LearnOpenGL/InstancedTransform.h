#pragma once


class InstancedTransform {
public:

	virtual std::vector<glm::vec3> * getPositions() = 0;

	virtual std::vector<glm::mat4> getModels() = 0;

	virtual void refreshModel() = 0;

	virtual void Draw() = 0;

private:

	virtual void readFile(std::string filename) = 0;
};