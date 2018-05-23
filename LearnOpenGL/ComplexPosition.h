#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "CompositionType.h"

class ComplexPosition
{
public:
	ComplexPosition();
	~ComplexPosition();

	std::vector<glm::vec3> getPositions(CompositionType i);
	void setPositions(std::vector<glm::vec3> positions, CompositionType i);

	std::vector<glm::vec3> getDrawablePositions(CompositionType i);
	void setDrawablePositions(std::vector<glm::vec3> drawablePositions, CompositionType i);

private:
	std::vector<std::vector<glm::vec3>> positions;
	std::vector<std::vector<glm::vec3>> drawablePositions;
};

