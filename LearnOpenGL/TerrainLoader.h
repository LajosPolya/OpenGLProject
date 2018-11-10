#pragma once


#include <mutex>

#include "Camera.h"
#include "TerrainGenerator.h"
#include "InstancedArrayTransformImpl.h"
#include "PositionRelativeCamera.h"

using namespace glm;

#define CHUNK_X (GLfloat)50
#define CHUNK_Y (GLfloat)25
#define CHUNK_Z (GLfloat)50

#define CUR_FORWARD_LEFT 0
#define CUR_FORWARD 1
#define CUR_FORWARD_RIGHT 2

class TerrainLoader
{
public:
	TerrainLoader(Camera * camera, mutex & returnQ_m, GLuint & readyToGrab, vector<PositionRelativeCamera> & returnQ);
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
		vec3 forwardLeft;
		vec3 forward;
		vec3 forwardRight;
	};


	thread t1;
	TerrainGenerator * terrainGenerator3d;
	Camera * camera;
	mutex & returnQ_m;
	GLuint & readyToGrab;
	vector<PositionRelativeCamera> & returnQ;

	GLint killAll = 0;

	// This represents a 3x3x3 grid viewed as a 1d array (the array storing all the GameObject in the main loop)
	vector<vector<GLint>> gridPositions;

	void Loader();

	/* Shift indexes and change Camera position */
	void shiftInPositiveXDir(vec3 pos);
	void shiftInNegativeXDir(vec3 pos);
	void shiftInPositiveZDir(vec3 pos);
	void shiftInNegativeZDir(vec3 pos);
	void shiftInPositiveXDir(vec3 pos, vector<GLint>& positions);
	void shiftInNegativeXDir(vec3 pos, vector<GLint>& positions);
	void shiftInPositiveZDir(vec3 pos, vector<GLint>& positions);
	void shiftInNegativeZDir(vec3 pos, vector<GLint>& positions);
	void shiftInPositiveYDir(vec3 pos);
	void shiftInNegativeYDir(vec3 pos);

	// Uses data produced in shiftIn*Dir() to call TerrainGenerator
	void generate(Position_To_Load positionsToLoad, vector<GLint> curPos);
};

