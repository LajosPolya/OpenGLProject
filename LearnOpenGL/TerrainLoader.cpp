#include "TerrainLoader.h"

// This implementation may not work if this class was a child class
// See: https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/

/*																																			ref & class members get initialized here*/
TerrainLoader::TerrainLoader(Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & returnQ) : returnQ_m(returnQ_m), readyToGrab(readyToGrab), returnQ(returnQ) {
	this->terrainGenerator3d = new TerrainGenerator((GLint)CHUNK_X, (GLint)CHUNK_Y, (GLint)CHUNK_Z, T_3D);
	this->camera = camera;
	this->gridPositions = { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, 
						{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
						{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } };
}

TerrainLoader::~TerrainLoader() {}

// `this` pointer also needs to be sent as parameter to thread
void TerrainLoader::start() {
	this->t1 = std::thread(&TerrainLoader::Loader, this);
}

void TerrainLoader::stop() {
	this->killAll = 1;
	this->t1.join();
}

void TerrainLoader::Loader() {
	GLint empty = 1;
	GLint done = 0;
	glm::vec3 prevPos = this->terrainGenerator3d->getChunkPos(this->camera->Position);
	std::vector<glm::vec3> messageQ;
	GLuint messageQSize = 0;

	// Preload data so the this->camera can move around while this loads
	messageQ.push_back(glm::vec3(this->camera->Position.x - CHUNK_X, this->camera->Position.y, this->camera->Position.z - CHUNK_Z));
	messageQ.push_back(glm::vec3(this->camera->Position.x - CHUNK_X, this->camera->Position.y, this->camera->Position.z + CHUNK_Z));
	messageQ.push_back(glm::vec3(this->camera->Position.x + CHUNK_X, this->camera->Position.y, this->camera->Position.z + CHUNK_Z));
	messageQ.push_back(glm::vec3(this->camera->Position.x - CHUNK_X, this->camera->Position.y, this->camera->Position.z));
	messageQ.push_back(glm::vec3(this->camera->Position.x, this->camera->Position.y, this->camera->Position.z + CHUNK_Z));
	messageQ.push_back(glm::vec3(this->camera->Position.x, this->camera->Position.y, this->camera->Position.z - CHUNK_Z));
	messageQ.push_back(glm::vec3(this->camera->Position.x + CHUNK_X, this->camera->Position.y, this->camera->Position.z - CHUNK_Z));
	messageQ.push_back(glm::vec3(this->camera->Position.x + CHUNK_X, this->camera->Position.y, this->camera->Position.z));
	messageQ.push_back(glm::vec3(this->camera->Position.x, this->camera->Position.y, this->camera->Position.z));

	std::cout << "Starting Init Generation" << std::endl;
	messageQSize = messageQ.size();
	for (GLuint i = 0; i < messageQSize; i++) {
		std::cout << i << std::endl;
		glm::vec3 initPos = messageQ[messageQ.size() - 1];
		messageQ.pop_back();
		this->terrainGenerator3d->generateComplex(initPos);
	}
	std::cout << "Ending Init Generation" << std::endl;
	std::cout << this->camera->Position.x << " " << this->camera->Position.y << " " << this->camera->Position.z << std::endl;

	this->returnQ_m.lock();
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(-CHUNK_X, 0, CHUNK_Z)).getDrawablePositions()), 8));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(-CHUNK_X, 0, 0)).getDrawablePositions()), 7));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(-CHUNK_X, 0, -CHUNK_Z)).getDrawablePositions()), 6));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(0, 0, CHUNK_Z)).getDrawablePositions()), 5));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position).getDrawablePositions()), 4));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(0, 0, -CHUNK_Z)).getDrawablePositions()), 3));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(CHUNK_X, 0, CHUNK_Z)).getDrawablePositions()), 2));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(CHUNK_X, 0, 0)).getDrawablePositions()), 1));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(CHUNK_X, 0, -CHUNK_Z)).getDrawablePositions()), 0));
	/* The below two lines need to be commented if we want Terrain to load with Camera movement */
	//this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(0, CHUNK_Y, 0)).getDrawablePositions()), 13));
	this->returnQ_m.unlock();
	this->readyToGrab = 1;
	while (this->killAll != 1) {
		glm::vec3 pos = this->camera->Position;
		glm::vec3 chunkPos = this->terrainGenerator3d->getChunkPos(pos);
		// Let's assume positive x is forward
		if (this->terrainGenerator3d->shouldGetNewChunks(pos) == 1 && chunkPos != prevPos && this->readyToGrab == 0) {
			std::cout << "  BEFORE  " << pos.x << " " << pos.z << " Chunk: " << chunkPos.x << " " << chunkPos.z << std::endl;
			std::cout << this->gridPositions[1][FRONT_LEFT] << " " << this->gridPositions[1][FRONT] << " " << this->gridPositions[1][FRONT_RIGHT] << std::endl;
			std::cout << this->gridPositions[1][LEFT] << " " << this->gridPositions[1][MIDDLE] << " " << this->gridPositions[1][RIGHT] << std::endl;
			std::cout << this->gridPositions[1][BACK_LEFT] << " " << this->gridPositions[1][BACK] << " " << this->gridPositions[1][BACK_RIGHT] << std::endl;
			
			// Is it safe to lock here?
			this->returnQ_m.lock();
			// TODO: Create a queue to queue up requests incase user moves too fast for generation
			if (chunkPos.x > prevPos.x && chunkPos.z > prevPos.z) {
				std::cout << "DIAGONAL" << std::endl;
				// Don't forget to add CHUNK_Y when that's implemented
				shiftInPositiveXDir(pos + glm::vec3(-CHUNK_X, 0, 0)); // Set to original position
				shiftInPositiveZDir(pos); // Now Use original position
			}
			else if (chunkPos.x > prevPos.x && chunkPos.z < prevPos.z) {
				std::cout << "DIAGONAL" << std::endl;
				shiftInPositiveXDir(pos + glm::vec3(-CHUNK_X, 0, 0));
				shiftInNegativeZDir(pos);
			}
			else if (chunkPos.x < prevPos.x && chunkPos.z > prevPos.z) {
				std::cout << "DIAGONAL" << std::endl;
				shiftInNegativeXDir(pos + glm::vec3(CHUNK_X, 0, 0));
				shiftInPositiveZDir(pos);
			}
			else if (chunkPos.x < prevPos.x && chunkPos.z < prevPos.z) {
				std::cout << "DIAGONAL" << std::endl;
				shiftInNegativeXDir(pos + glm::vec3(CHUNK_X, 0, 0));
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
			else {
				std::cout << "ERROR in TerrainLoader" << std::endl;
			}
			this->returnQ_m.unlock();
			// Tells main that it can grab the newly loaded Terrain
			this->readyToGrab = 1;

			std::cout << "  AFTER  " << std::endl;
			std::cout << this->gridPositions[1][FRONT_LEFT] << " " << this->gridPositions[1][FRONT] << " " << this->gridPositions[1][FRONT_RIGHT] << std::endl;
			std::cout << this->gridPositions[1][LEFT] << " " << this->gridPositions[1][MIDDLE] << " " << this->gridPositions[1][RIGHT] << std::endl;
			std::cout << this->gridPositions[1][BACK_LEFT] << " " << this->gridPositions[1][BACK] << " " << this->gridPositions[1][BACK_RIGHT] << std::endl;
			std::cout << std::endl << std::endl;
			prevPos = chunkPos;
		}
		else if (this->terrainGenerator3d->shouldGetNewChunks(pos) == 1 && chunkPos == prevPos) {
			std::cout << "WARNING::TerrainLoader:: prevPosition == chunkPositoin" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
}

void TerrainLoader::shiftInPositiveXDir(glm::vec3 pos) {
	shiftInPositiveXDir(pos, this->gridPositions[1]);
}

void TerrainLoader::shiftInNegativeXDir(glm::vec3 pos) {
	shiftInNegativeXDir(pos, this->gridPositions[1]);
}

void TerrainLoader::shiftInPositiveZDir(glm::vec3 pos) {
	shiftInPositiveZDir(pos, this->gridPositions[1]);
}

void TerrainLoader::shiftInNegativeZDir(glm::vec3 pos) {
	shiftInNegativeZDir(pos, this->gridPositions[1]);
}

void TerrainLoader::shiftInPositiveXDir(glm::vec3 pos, std::vector<GLint>& gridPositions){
	std::vector<GLint> curGridPos(3);
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

	positionsToLoad.forwardLeft = glm::vec3(pos.x + CHUNK_X, pos.y, pos.z - CHUNK_Z);
	positionsToLoad.forward = glm::vec3(pos.x + CHUNK_X, pos.y, pos.z);
	positionsToLoad.forwardRight = glm::vec3(pos.x + CHUNK_X, pos.y, pos.z + CHUNK_Z);

	generate(positionsToLoad, curGridPos);
}

void TerrainLoader::shiftInNegativeXDir(glm::vec3 pos, std::vector<GLint>& gridPositions) {
	std::vector<GLint> curGridPos(3);
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

	positionsToLoad.forwardLeft = glm::vec3(pos.x - CHUNK_X, pos.y, pos.z - CHUNK_Z);
	positionsToLoad.forward = glm::vec3(pos.x - CHUNK_X, pos.y, pos.z);
	positionsToLoad.forwardRight = glm::vec3(pos.x - CHUNK_X, pos.y, pos.z + CHUNK_Z);

	generate(positionsToLoad, curGridPos);
}

void TerrainLoader::shiftInPositiveZDir(glm::vec3 pos, std::vector<GLint>& gridPositions) {
	std::vector<GLint> curGridPos(3);
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

	positionsToLoad.forwardLeft = glm::vec3(pos.x + CHUNK_X, pos.y, pos.z + CHUNK_Z);
	positionsToLoad.forward = glm::vec3(pos.x, pos.y, pos.z + CHUNK_Z);
	positionsToLoad.forwardRight = glm::vec3(pos.x - CHUNK_X, pos.y, pos.z + CHUNK_Z);

	generate(positionsToLoad, curGridPos);
}

void TerrainLoader::shiftInNegativeZDir(glm::vec3 pos, std::vector<GLint>& gridPositions) {
	std::vector<GLint> curGridPos(3);
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

	positionsToLoad.forwardLeft = glm::vec3(pos.x + CHUNK_X, pos.y, pos.z - CHUNK_Z);
	positionsToLoad.forward = glm::vec3(pos.x, pos.y, pos.z - CHUNK_Z);
	positionsToLoad.forwardRight = glm::vec3(pos.x - CHUNK_X, pos.y, pos.z - CHUNK_Z);

	generate(positionsToLoad, curGridPos);
}

void TerrainLoader::generate(Position_To_Load positionsToLoad, std::vector<GLint> curGridPos) {
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(positionsToLoad.forwardLeft).getDrawablePositions()), curGridPos[CUR_FORWARD_LEFT]));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(positionsToLoad.forward).getDrawablePositions()), curGridPos[CUR_FORWARD]));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(positionsToLoad.forwardRight).getDrawablePositions()), curGridPos[CUR_FORWARD_RIGHT]));
}
