#pragma once

#include <vector>

#include "SimpleInstancedArrayGameObject.h"
#include "CompositionType.h"

class ChunkObject
{
public:
	ChunkObject();
	~ChunkObject();

	SimpleInstancedArrayGameObject & getChunkObject(CompositionType i);

private:
	std::vector<SimpleInstancedArrayGameObject> composition;
};

