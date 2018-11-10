#pragma once

#include <vector>

#include "SimpleInstancedArrayGameObject.h"
#include "CompositionType.h"
#include "PositionRelativeCamera.h"

using namespace std;

class ChunkObject
{
public:
	ChunkObject();
	~ChunkObject();

	void DrawComponents();
	void setInstances(PositionRelativeCamera pos);

private:
	vector<SimpleInstancedArrayGameObject> composition;
};

