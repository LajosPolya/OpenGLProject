#pragma once


#include <mutex>

#include "Camera.h"
#include "TerrainGenerator.h"
#include "InstancedArrayTransformImpl.h"
#include "PositionRelativeCamera.h"

#define CHUNK_X (GLfloat)50
#define CHUNK_Y (GLfloat)25
#define CHUNK_Z (GLfloat)50

#define CUR_FORWARD_LEFT 0
#define CUR_FORWARD 1
#define CUR_FORWARD_RIGHT 2

class TerrainLoader
{
public:
	TerrainLoader(Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & returnQ);
	~TerrainLoader();

	void start();
	void stop();

private:
	enum Position {
		FRONT_LEFT,
		FRONT,
		FRONT_RIGHT,
		LEFT,
		MIDDLE,
		RIGHT,
		BACK_LEFT,
		BACK = 7,
		BACK_RIGHT
	};

	struct Position_To_Load {
		glm::vec3 forwardLeft;
		glm::vec3 forward;
		glm::vec3 forwardRight;
	};


	std::thread t1;
	TerrainGenerator * terrainGenerator3d;
	Camera * camera;
	std::mutex & returnQ_m;
	GLuint & readyToGrab;
	std::vector<PositionRelativeCamera> & returnQ;

	GLint killAll = 0;

	// This represents a 3x3x3 grid viewed as a 1d array (the array storing all the GameObject in the main loop)
	std::vector<std::vector<GLint>> gridPositions;

	void Loader();

	/* Shift indexes and change Camera position */
	void shiftInPositiveXDir(glm::vec3 pos);
	void shiftInNegativeXDir(glm::vec3 pos);
	void shiftInPositiveZDir(glm::vec3 pos);
	void shiftInNegativeZDir(glm::vec3 pos);
	void shiftInPositiveXDir(glm::vec3 pos, std::vector<GLint>& positions);
	void shiftInNegativeXDir(glm::vec3 pos, std::vector<GLint>& positions);
	void shiftInPositiveZDir(glm::vec3 pos, std::vector<GLint>& positions);
	void shiftInNegativeZDir(glm::vec3 pos, std::vector<GLint>& positions);

	// Uses data produced in shiftIn*Dir() to call TerrainGenerator
	void generate(Position_To_Load positionsToLoad, std::vector<GLint> curPos);
};

