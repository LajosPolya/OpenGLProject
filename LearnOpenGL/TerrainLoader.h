#pragma once


#include <mutex>

#include "Camera.h"
#include "TerrainGenerator.h"
#include "InstancedArrayTransformImpl.h"
#include "PositionRelativeCamera.h"

#define CHUNK_X (GLfloat)50
#define CHUNK_Y (GLfloat)25
#define CHUNK_Z (GLfloat)50

class TerrainLoader
{
public:
	TerrainLoader(Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & returnQ);
	~TerrainLoader();

	void start();
	void stop();

private:
	/* Implement this to only call terrain geenrator in Load() */
	struct Position_To_Load {
		glm::vec3 forwardLeft;
		glm::vec3 forward;
		glm::vec3 forwardRight;
	} positionsToLoad; // positionsToLoad is an instance of type Position_To_Load


	std::thread t1;
	TerrainGenerator * terrainGenerator3d;
	Camera * camera;
	std::mutex & returnQ_m;
	GLuint & readyToGrab;
	std::vector<PositionRelativeCamera> & returnQ;

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

	void Loader();

	/* Shift indexes and change Camera position */
	void shiftInPositiveXDir(glm::vec3 pos);
	void shiftInNegativeXDir(glm::vec3 pos);
	void shiftInPositiveZDir(glm::vec3 pos);
	void shiftInNegativeZDir(glm::vec3 pos);
};

