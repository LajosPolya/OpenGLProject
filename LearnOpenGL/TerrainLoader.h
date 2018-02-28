#pragma once


#include <mutex>

#include "Camera.h"
#include "TerrainGenerator.h"
#include "InstancedArrayTransformImpl.h"
#include "PositionRelativeCamera.h"

class TerrainLoader
{
public:
	TerrainLoader(GLint chunkX, GLint chunkY, GLint chunkZ, TerrainGenerator & terrainGenerator3d, Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & returnQ);
	~TerrainLoader();

	void start();
	void stop();

private:
	std::thread t1;
	TerrainGenerator & terrainGenerator3d;
	Camera * camera;
	std::mutex & returnQ_m;
	GLuint & readyToGrab;
	std::vector<PositionRelativeCamera> & returnQ;


	GLint CHUNK_X, CHUNK_Y, CHUNK_Z;
	GLint killAll = 0;

	// These vars represent a 3x3 grid viewed as a 1d array (the array storing all the GameObject in the main loop)
	GLint FRONT_LEFT = 0;
	GLint FRONT = 1;
	GLint FRONT_RIGHT = 2;
	GLint LEFT = 3;
	GLint MIDDLE = 4;
	GLint RIGHT = 5;
	GLint BACK_LEFT = 6;
	GLint BACK = 7;
	GLint BACK_RIGHT = 8;

	GLint CUR_FORWARD_LEFT;
	GLint CUR_FORWARD;
	GLint CUR_FORWARD_RIGHT;

	void Loader(TerrainGenerator & terrainGenerator3d, Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & returnQ);

	/* Shift indexes and change Camera position */
	void shiftInPositiveXDir(glm::vec3 pos);
	void shiftInNegativeXDir(glm::vec3 pos);
	void shiftInPositiveZDir(glm::vec3 pos);
	void shiftInNegativeZDir(glm::vec3 pos);
};

