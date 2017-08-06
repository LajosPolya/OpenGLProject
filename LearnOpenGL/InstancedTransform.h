#pragma once

class InstancedTransform {
public:
	virtual std::vector<glm::mat4> getModels() = 0;

	virtual void refreshModel() = 0;

	virtual void Draw() = 0;

private:
	virtual void readFile(GLchar* filename) = 0;
};