#pragma once

#include <vector>

#include "SimpleInstancedArrayGameObject.h"
#include "CompositionType.h"
#include "PositionRelativeCamera.h"

class ChunkObject
{
public:
	ChunkObject();
	~ChunkObject();

	void DrawComponents();
	void setInstances(PositionRelativeCamera pos);

private:
	std::vector<SimpleInstancedArrayGameObject> composition;
};

