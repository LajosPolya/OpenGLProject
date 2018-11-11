#include "TerrainLoader.h"

// This implementation may not work if this class was a child class
// See: https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/

/*																																			ref & class members get initialized here*/
TerrainLoader::TerrainLoader(Camera * camera, mutex & returnQ_m, GLuint & readyToGrab, vector<PositionRelativeCamera> & returnQ) : returnQ_m(returnQ_m), readyToGrab(readyToGrab), returnQ(returnQ) {
	this->terrainGenerator3d = new TerrainGenerator((GLint)CHUNK_X, (GLint)CHUNK_Y, (GLint)CHUNK_Z, T_3D);
	this->camera = camera;
	this->gridPositions = { { 0, 1, 2, 3, 4, 5, 6, 7, 8 }, 
						{ 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 },
						/*{ 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 }*/ };
}

TerrainLoader::~TerrainLoader() {}

// `this` pointer also needs to be sent as parameter to thread
void TerrainLoader::start() {
	this->t1 = thread(&TerrainLoader::Loader, this);
}

void TerrainLoader::stop() {
	this->killAll = 1;
	this->t1.join();
}

void TerrainLoader::Loader() {
	GLint empty = 1;
	GLint done = 0;
	vec3 prevPos = this->terrainGenerator3d->getChunkPos(this->camera->getPosition());
	vector<vec3> messageQ;
	GLuint messageQSize = 0;

	const vec3 & cPos = this->camera->getPosition();

	// Preload data so the this->camera can move around while this loads
	messageQ.push_back(vec3(cPos.x - CHUNK_X, cPos.y, cPos.z - CHUNK_Z));
	messageQ.push_back(vec3(cPos.x - CHUNK_X, cPos.y, cPos.z + CHUNK_Z));
	messageQ.push_back(vec3(cPos.x + CHUNK_X, cPos.y, cPos.z + CHUNK_Z));
	messageQ.push_back(vec3(cPos.x - CHUNK_X, cPos.y, cPos.z));
	messageQ.push_back(vec3(cPos.x, cPos.y, cPos.z + CHUNK_Z));
	messageQ.push_back(vec3(cPos.x, cPos.y, cPos.z - CHUNK_Z));
	messageQ.push_back(vec3(cPos.x + CHUNK_X, cPos.y, cPos.z - CHUNK_Z));
	messageQ.push_back(vec3(cPos.x + CHUNK_X, cPos.y, cPos.z));
	messageQ.push_back(vec3(cPos.x, cPos.y, cPos.z));

	messageQ.push_back(vec3(cPos.x - CHUNK_X, cPos.y + CHUNK_Y, cPos.z - CHUNK_Z));
	messageQ.push_back(vec3(cPos.x - CHUNK_X, cPos.y + CHUNK_Y, cPos.z + CHUNK_Z));
	messageQ.push_back(vec3(cPos.x + CHUNK_X, cPos.y + CHUNK_Y, cPos.z + CHUNK_Z));
	messageQ.push_back(vec3(cPos.x - CHUNK_X, cPos.y + CHUNK_Y, cPos.z));
	messageQ.push_back(vec3(cPos.x, cPos.y + CHUNK_Y, cPos.z + CHUNK_Z));
	messageQ.push_back(vec3(cPos.x, cPos.y + CHUNK_Y, cPos.z - CHUNK_Z));
	messageQ.push_back(vec3(cPos.x + CHUNK_X, cPos.y + CHUNK_Y, cPos.z - CHUNK_Z));
	messageQ.push_back(vec3(cPos.x + CHUNK_X, cPos.y + CHUNK_Y, cPos.z));
	messageQ.push_back(vec3(cPos.x, cPos.y + CHUNK_Y, cPos.z));

	cout << "Starting Init Generation" << endl;
	messageQSize = messageQ.size();
	for (GLuint i = 0; i < messageQSize; i++) {
		cout << i << endl;
		vec3 initPos = messageQ[messageQ.size() - 1];
		messageQ.pop_back();
		this->terrainGenerator3d->generateComplex(initPos);
	}
	cout << "Ending Init Generation" << endl;
	cout << cPos.x << " " << cPos.y << " " << cPos.z << endl;

	this->returnQ_m.lock();
	// Middle
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(-CHUNK_X, 0, CHUNK_Z))), 8));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(-CHUNK_X, 0, 0))), 7));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(-CHUNK_X, 0, -CHUNK_Z))), 6));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(0, 0, CHUNK_Z))), 5));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos)), 4));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(0, 0, -CHUNK_Z))), 3));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(CHUNK_X, 0, CHUNK_Z))), 2));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(CHUNK_X, 0, 0))), 1));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(CHUNK_X, 0, -CHUNK_Z))), 0));

	// Top
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(-CHUNK_X, CHUNK_Y, CHUNK_Z))), 17));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(-CHUNK_X, CHUNK_Y, 0))), 16));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(-CHUNK_X, CHUNK_Y, -CHUNK_Z))), 15));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(0, CHUNK_Y, CHUNK_Z))), 14));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(0, CHUNK_Y, 0))), 13));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(0, CHUNK_Y, -CHUNK_Z))), 12));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(CHUNK_X, CHUNK_Y, CHUNK_Z))), 11));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(CHUNK_X, CHUNK_Y, 0))), 10));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(cPos + vec3(CHUNK_X, CHUNK_Y, -CHUNK_Z))), 9));
	/* The below two lines need to be commented if we want Terrain to load with Camera movement */
	//this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(cPos + vec3(0, CHUNK_Y, 0)).getDrawablePositions(GRASS)), 13));
	this->returnQ_m.unlock();
	this->readyToGrab = 1;
	while (this->killAll != 1) {
		vec3 pos = cPos;
		vec3 chunkPos = this->terrainGenerator3d->getChunkPos(pos);
		// Let's assume positive x is forward
		if (this->terrainGenerator3d->shouldGetNewChunks(pos) == 1 && chunkPos != prevPos && this->readyToGrab == 0) {
			cout << "  BEFORE  " << pos.x << " " << pos.z << " Chunk: " << chunkPos.x << " " << chunkPos.z << endl;
			cout << this->gridPositions[1][FRONT_LEFT] << " " << this->gridPositions[1][FRONT] << " " << this->gridPositions[1][FRONT_RIGHT] << endl;
			cout << this->gridPositions[1][LEFT] << " " << this->gridPositions[1][MIDDLE] << " " << this->gridPositions[1][RIGHT] << endl;
			cout << this->gridPositions[1][BACK_LEFT] << " " << this->gridPositions[1][BACK] << " " << this->gridPositions[1][BACK_RIGHT] << endl;
			
			// Is it safe to lock here?
			this->returnQ_m.lock();
			// TODO: Create a queue to queue up requests incase user moves too fast for generation
			if (chunkPos.x > prevPos.x && chunkPos.z > prevPos.z) {
				cout << "DIAGONAL" << endl;
				// Don't forget to add CHUNK_Y when that's implemented
				shiftInPositiveXDir(pos + vec3(-CHUNK_X, 0, 0)); // Set to original position
				shiftInPositiveZDir(pos); // Now Use original position
			}
			else if (chunkPos.x > prevPos.x && chunkPos.z < prevPos.z) {
				cout << "DIAGONAL" << endl;
				shiftInPositiveXDir(pos + vec3(-CHUNK_X, 0, 0));
				shiftInNegativeZDir(pos);
			}
			else if (chunkPos.x < prevPos.x && chunkPos.z > prevPos.z) {
				cout << "DIAGONAL" << endl;
				shiftInNegativeXDir(pos + vec3(CHUNK_X, 0, 0));
				shiftInPositiveZDir(pos);
			}
			else if (chunkPos.x < prevPos.x && chunkPos.z < prevPos.z) {
				cout << "DIAGONAL" << endl;
				shiftInNegativeXDir(pos + vec3(CHUNK_X, 0, 0));
				shiftInNegativeZDir(pos);
			}
			else if (chunkPos.x > prevPos.x) {
				shiftInPositiveXDir(pos);
			}
			else if (chunkPos.x < prevPos.x) {
				shiftInNegativeXDir(pos);
			}
			else if (chunkPos.z > prevPos.z) {
				shiftInPositiveZDir(pos);
			}
			else if (chunkPos.z < prevPos.z) {
				shiftInNegativeZDir(pos);
			}
			else if (chunkPos.y > prevPos.y) {
				shiftInPositiveYDir(pos);
			}
			else if (chunkPos.y < prevPos.y) {
				shiftInNegativeYDir(pos);
			}
			else {
				cout << "ERROR in TerrainLoader" << endl;
			}
			this->returnQ_m.unlock();
			// Tells main that it can grab the newly loaded Terrain
			this->readyToGrab = 1;

			cout << "  AFTER  " << endl;
			cout << this->gridPositions[1][FRONT_LEFT] << " " << this->gridPositions[1][FRONT] << " " << this->gridPositions[1][FRONT_RIGHT] << endl;
			cout << this->gridPositions[1][LEFT] << " " << this->gridPositions[1][MIDDLE] << " " << this->gridPositions[1][RIGHT] << endl;
			cout << this->gridPositions[1][BACK_LEFT] << " " << this->gridPositions[1][BACK] << " " << this->gridPositions[1][BACK_RIGHT] << endl;
			cout << endl << endl;
			prevPos = chunkPos;
		}
		else if (this->terrainGenerator3d->shouldGetNewChunks(pos) == 1 && chunkPos == prevPos) {
			cout << "WARNING::TerrainLoader:: prevPosition == chunkPositoin" << endl;
			this_thread::sleep_for(chrono::milliseconds(50));
		}
		else {
			this_thread::sleep_for(chrono::milliseconds(50));
		}
	}
}

void TerrainLoader::shiftInPositiveXDir(vec3 pos) {
	shiftInPositiveXDir(pos, this->gridPositions[0]);
	shiftInPositiveXDir(pos + vec3{ 0, CHUNK_Y, 0 }, this->gridPositions[1]);
}

void TerrainLoader::shiftInNegativeXDir(vec3 pos) {
	shiftInNegativeXDir(pos, this->gridPositions[0]);
	shiftInNegativeXDir(pos + vec3{ 0, CHUNK_Y, 0 }, this->gridPositions[1]);
}

void TerrainLoader::shiftInPositiveZDir(vec3 pos) {
	shiftInPositiveZDir(pos, this->gridPositions[0]);
	shiftInPositiveZDir(pos + vec3{ 0, CHUNK_Y, 0 }, this->gridPositions[1]);
}

void TerrainLoader::shiftInNegativeZDir(vec3 pos) {
	shiftInNegativeZDir(pos, this->gridPositions[0]);
	shiftInNegativeZDir(pos + vec3{ 0, CHUNK_Y, 0 }, this->gridPositions[1]);
}

void TerrainLoader::shiftInPositiveXDir(vec3 pos, vector<GLint>& gridPositions){
	vector<GLint> curGridPos(3);
	Position_To_Load positionsToLoad;

	curGridPos[CUR_FORWARD_LEFT] = gridPositions[BACK_LEFT];
	curGridPos[CUR_FORWARD] = gridPositions[BACK];
	curGridPos[CUR_FORWARD_RIGHT] = gridPositions[BACK_RIGHT];

	gridPositions[BACK_LEFT] = gridPositions[LEFT];
	gridPositions[BACK] = gridPositions[MIDDLE];
	gridPositions[BACK_RIGHT] = gridPositions[RIGHT];

	gridPositions[LEFT] = gridPositions[FRONT_LEFT];
	gridPositions[MIDDLE] = gridPositions[FRONT];
	gridPositions[RIGHT] = gridPositions[FRONT_RIGHT];

	gridPositions[FRONT_LEFT] = curGridPos[CUR_FORWARD_LEFT];
	gridPositions[FRONT] = curGridPos[CUR_FORWARD];
	gridPositions[FRONT_RIGHT] = curGridPos[CUR_FORWARD_RIGHT];

	positionsToLoad.forwardLeft = vec3(pos.x + CHUNK_X, pos.y, pos.z - CHUNK_Z);
	positionsToLoad.forward = vec3(pos.x + CHUNK_X, pos.y, pos.z);
	positionsToLoad.forwardRight = vec3(pos.x + CHUNK_X, pos.y, pos.z + CHUNK_Z);

	generate(positionsToLoad, curGridPos);
}

void TerrainLoader::shiftInNegativeXDir(vec3 pos, vector<GLint>& gridPositions) {
	vector<GLint> curGridPos(3);
	Position_To_Load positionsToLoad;

	curGridPos[CUR_FORWARD_LEFT] = gridPositions[FRONT_LEFT];
	curGridPos[CUR_FORWARD] = gridPositions[FRONT];
	curGridPos[CUR_FORWARD_RIGHT] = gridPositions[FRONT_RIGHT];

	gridPositions[FRONT_LEFT] = gridPositions[LEFT];
	gridPositions[FRONT] = gridPositions[MIDDLE];
	gridPositions[FRONT_RIGHT] = gridPositions[RIGHT];

	gridPositions[LEFT] = gridPositions[BACK_LEFT];
	gridPositions[MIDDLE] = gridPositions[BACK];
	gridPositions[RIGHT] = gridPositions[BACK_RIGHT];

	gridPositions[BACK_LEFT] = curGridPos[CUR_FORWARD_LEFT];
	gridPositions[BACK] = curGridPos[CUR_FORWARD];
	gridPositions[BACK_RIGHT] = curGridPos[CUR_FORWARD_RIGHT];

	positionsToLoad.forwardLeft = vec3(pos.x - CHUNK_X, pos.y, pos.z - CHUNK_Z);
	positionsToLoad.forward = vec3(pos.x - CHUNK_X, pos.y, pos.z);
	positionsToLoad.forwardRight = vec3(pos.x - CHUNK_X, pos.y, pos.z + CHUNK_Z);

	generate(positionsToLoad, curGridPos);
}

void TerrainLoader::shiftInPositiveZDir(vec3 pos, vector<GLint>& gridPositions) {
	vector<GLint> curGridPos(3);
	Position_To_Load positionsToLoad;

	curGridPos[CUR_FORWARD_LEFT] = gridPositions[FRONT_LEFT];
	curGridPos[CUR_FORWARD] = gridPositions[LEFT];
	curGridPos[CUR_FORWARD_RIGHT] = gridPositions[BACK_LEFT];

	gridPositions[FRONT_LEFT] = gridPositions[FRONT];
	gridPositions[LEFT] = gridPositions[MIDDLE];
	gridPositions[BACK_LEFT] = gridPositions[BACK];

	gridPositions[FRONT] = gridPositions[FRONT_RIGHT];
	gridPositions[MIDDLE] = gridPositions[RIGHT];
	gridPositions[BACK] = gridPositions[BACK_RIGHT];

	gridPositions[FRONT_RIGHT] = curGridPos[CUR_FORWARD_LEFT];
	gridPositions[RIGHT] = curGridPos[CUR_FORWARD];
	gridPositions[BACK_RIGHT] = curGridPos[CUR_FORWARD_RIGHT];

	positionsToLoad.forwardLeft = vec3(pos.x + CHUNK_X, pos.y, pos.z + CHUNK_Z);
	positionsToLoad.forward = vec3(pos.x, pos.y, pos.z + CHUNK_Z);
	positionsToLoad.forwardRight = vec3(pos.x - CHUNK_X, pos.y, pos.z + CHUNK_Z);

	generate(positionsToLoad, curGridPos);
}

void TerrainLoader::shiftInNegativeZDir(vec3 pos, vector<GLint>& gridPositions) {
	vector<GLint> curGridPos(3);
	Position_To_Load positionsToLoad;

	curGridPos[CUR_FORWARD_LEFT] = gridPositions[FRONT_RIGHT];
	curGridPos[CUR_FORWARD] = gridPositions[RIGHT];
	curGridPos[CUR_FORWARD_RIGHT] = gridPositions[BACK_RIGHT];

	gridPositions[FRONT_RIGHT] = gridPositions[FRONT];
	gridPositions[RIGHT] = gridPositions[MIDDLE];
	gridPositions[BACK_RIGHT] = gridPositions[BACK];

	gridPositions[FRONT] = gridPositions[FRONT_LEFT];
	gridPositions[MIDDLE] = gridPositions[LEFT];
	gridPositions[BACK] = gridPositions[BACK_LEFT];

	gridPositions[FRONT_LEFT] = curGridPos[CUR_FORWARD_LEFT];
	gridPositions[LEFT] = curGridPos[CUR_FORWARD];
	gridPositions[BACK_LEFT] = curGridPos[CUR_FORWARD_RIGHT];

	positionsToLoad.forwardLeft = vec3(pos.x + CHUNK_X, pos.y, pos.z - CHUNK_Z);
	positionsToLoad.forward = vec3(pos.x, pos.y, pos.z - CHUNK_Z);
	positionsToLoad.forwardRight = vec3(pos.x - CHUNK_X, pos.y, pos.z - CHUNK_Z);

	generate(positionsToLoad, curGridPos);
}

void TerrainLoader::shiftInPositiveYDir(vec3 pos) {
	vector<GLint> temp = this->gridPositions[0];
	this->gridPositions[0] = this->gridPositions[1];
	this->gridPositions[1] = temp;

	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(CHUNK_X, CHUNK_Y, -CHUNK_Z))), this->gridPositions[1][FRONT_LEFT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(CHUNK_X, CHUNK_Y, 0))), this->gridPositions[1][FRONT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(CHUNK_X, CHUNK_Y, CHUNK_Z))), this->gridPositions[1][FRONT_RIGHT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(0, CHUNK_Y, -CHUNK_Z))), this->gridPositions[1][LEFT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(0, CHUNK_Y, 0))), this->gridPositions[1][MIDDLE]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(0, CHUNK_Y, CHUNK_Z))), this->gridPositions[1][RIGHT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(-CHUNK_X, CHUNK_Y, -CHUNK_Z))), this->gridPositions[1][BACK_LEFT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(-CHUNK_X, CHUNK_Y, 0))), this->gridPositions[1][BACK]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(-CHUNK_X, CHUNK_Y, CHUNK_Z))), this->gridPositions[1][BACK_RIGHT]));
}

void TerrainLoader::shiftInNegativeYDir(vec3 pos) {
	vector<GLint> temp = this->gridPositions[1];
	this->gridPositions[1] = this->gridPositions[0];
	this->gridPositions[0] = temp;

	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(CHUNK_X, 0, -CHUNK_Z))), this->gridPositions[0][FRONT_LEFT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(CHUNK_X, 0, 0))), this->gridPositions[0][FRONT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(CHUNK_X, 0, CHUNK_Z))), this->gridPositions[0][FRONT_RIGHT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(0, 0, -CHUNK_Z))), this->gridPositions[0][LEFT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(0, 0, 0))), this->gridPositions[0][MIDDLE]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(0, 0, CHUNK_Z))), this->gridPositions[0][RIGHT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(-CHUNK_X, 0, -CHUNK_Z))), this->gridPositions[0][BACK_LEFT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(-CHUNK_X, 0, 0))), this->gridPositions[0][BACK]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(pos + vec3(-CHUNK_X, 0, CHUNK_Z))), this->gridPositions[0][BACK_RIGHT]));
}

void TerrainLoader::generate(Position_To_Load positionsToLoad, vector<GLint> curGridPos) {
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(positionsToLoad.forwardLeft)), curGridPos[CUR_FORWARD_LEFT]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(positionsToLoad.forward)), curGridPos[CUR_FORWARD]));
	this->returnQ.push_back(PositionRelativeCamera(ChunkObjectTransformStore(this->terrainGenerator3d->generateComplex(positionsToLoad.forwardRight)), curGridPos[CUR_FORWARD_RIGHT]));
}
