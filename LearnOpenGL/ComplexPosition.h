#pragma once

#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ComplexPosition
{
public:
	ComplexPosition();
	~ComplexPosition();

	std::vector<glm::vec3> getPositions();
	void setPositions(std::vector<glm::vec3> positions);

	std::vector<glm::vec3> getDrawablePositions();
	void setDrawablePositions(std::vector<glm::vec3> drawablePositions);

private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> drawablePositions;
};

