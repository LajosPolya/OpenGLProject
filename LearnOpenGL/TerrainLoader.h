#pragma once


#include <mutex>

#include "Camera.h"
#include "TerrainGenerator.h"
#include "InstancedArrayTransformImpl.h"
#include "PositionRelativeCamera.h"

class TerrainLoader
{
public:
	TerrainLoader(GLint chunkX, GLint chunkY, GLint chunkZ);
	~TerrainLoader();

	void start(TerrainGenerator & terrainGenerator3d, Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & returnQ);
	void stop();

private:
	std::thread t1;
	GLint CHUNK_X, CHUNK_Y, CHUNK_Z;
	GLint killAll = 0;

	void Loader(TerrainGenerator & terrainGenerator3d, Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & returnQ);
};

