#include "TerrainLoader.h"

// This implementation may not work if this class was a child class
// See: https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/

/*																																			ref & class members get initialized here*/
TerrainLoader::TerrainLoader(Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & returnQ) : returnQ_m(returnQ_m), readyToGrab(readyToGrab), returnQ(returnQ) {
	this->terrainGenerator3d = new TerrainGenerator((GLint)CHUNK_X, (GLint)CHUNK_Y, (GLint)CHUNK_Z, T_3D);
	this->camera = camera;
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
	glm::vec3 prevPos = this->camera->Position;
	std::vector<glm::vec3> messageQ;
	GLuint messageQSize = 0;

	// Preload data so the this->camera can move around while this loads
	messageQ.push_back(glm::vec3(this->camera->Position.x - 50.0, this->camera->Position.y, this->camera->Position.z - 50.0));
	messageQ.push_back(glm::vec3(this->camera->Position.x - 50.0, this->camera->Position.y, this->camera->Position.z + 50.0));
	messageQ.push_back(glm::vec3(this->camera->Position.x + 50.0, this->camera->Position.y, this->camera->Position.z + 50.0));
	messageQ.push_back(glm::vec3(this->camera->Position.x - 50.0, this->camera->Position.y, this->camera->Position.z));
	messageQ.push_back(glm::vec3(this->camera->Position.x, this->camera->Position.y, this->camera->Position.z + 50.0));
	messageQ.push_back(glm::vec3(this->camera->Position.x, this->camera->Position.y, this->camera->Position.z - 50.0));
	messageQ.push_back(glm::vec3(this->camera->Position.x + 50.0, this->camera->Position.y, this->camera->Position.z - 50.0));
	messageQ.push_back(glm::vec3(this->camera->Position.x + 50.0, this->camera->Position.y, this->camera->Position.z));
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
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(-50, 0, 50)).getDrawablePositions()), 8));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(-50, 0, 0)).getDrawablePositions()), 7));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(-50, 0, -50)).getDrawablePositions()), 6));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(0, 0, 50)).getDrawablePositions()), 5));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position).getDrawablePositions()), 4));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(0, 0, -50)).getDrawablePositions()), 3));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(50, 0, 50)).getDrawablePositions()), 2));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(50, 0, 0)).getDrawablePositions()), 1));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->camera->Position + glm::vec3(50, 0, -50)).getDrawablePositions()), 0));
	this->returnQ_m.unlock();
	this->readyToGrab = 1;
	while (this->killAll != 1) {
		glm::vec3 pos = this->camera->Position;
		glm::vec3 chunkPos = this->terrainGenerator3d->getChunkPos(pos);
		// Let's assume positive x is forward
		if (this->terrainGenerator3d->shouldGetNewChunks(pos) == 1 && chunkPos != prevPos && this->readyToGrab == 0) {
			std::cout << "  BEFORE  " << pos.x << " " << pos.z << " Chunk: " << chunkPos.x << " " << chunkPos.z << std::endl;
			std::cout << this->FRONT_LEFT << " " << this->FRONT << " " << this->FRONT_RIGHT << std::endl;
			std::cout << this->LEFT << " " << this->MIDDLE << " " << this->RIGHT << std::endl;
			std::cout << this->BACK_LEFT << " " << this->BACK << " " << this->BACK_RIGHT << std::endl;
			
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
			std::cout << this->FRONT_LEFT << " " << this->FRONT << " " << this->FRONT_RIGHT << std::endl;
			std::cout << this->LEFT << " " << this->MIDDLE << " " << this->RIGHT << std::endl;
			std::cout << this->BACK_LEFT << " " << this->BACK << " " << this->BACK_RIGHT << std::endl;
			std::cout << std::endl << std::endl;
			prevPos = chunkPos;
		}
		else if (chunkPos == prevPos) {
			std::cout << "WARNING::TerrainLoader:: prevPosition == chunkPositoin" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
}

void TerrainLoader::shiftInPositiveXDir(glm::vec3 pos) {
	this->CUR_FORWARD_LEFT = this->BACK_LEFT;
	this->CUR_FORWARD = this->BACK;
	this->CUR_FORWARD_RIGHT = this->BACK_RIGHT;

	this->BACK_LEFT = this->LEFT;
	this->BACK = this->MIDDLE;
	this->BACK_RIGHT = this->RIGHT;

	this->LEFT = this->FRONT_LEFT;
	this->MIDDLE = this->FRONT;
	this->RIGHT = this->FRONT_RIGHT;

	this->FRONT_LEFT = this->CUR_FORWARD_LEFT;
	this->FRONT = this->CUR_FORWARD;
	this->FRONT_RIGHT = this->CUR_FORWARD_RIGHT;

	this->positionsToLoad.forwardLeft = glm::vec3(pos.x + CHUNK_X, pos.y, pos.z - CHUNK_Z);
	this->positionsToLoad.forward = glm::vec3(pos.x + CHUNK_X, pos.y, pos.z);
	this->positionsToLoad.forwardRight = glm::vec3(pos.x + CHUNK_X, pos.y, pos.z + CHUNK_Z);

	generate();
}

void TerrainLoader::shiftInNegativeXDir(glm::vec3 pos) {
	this->CUR_FORWARD_LEFT = this->FRONT_LEFT;
	this->CUR_FORWARD = this->FRONT;
	this->CUR_FORWARD_RIGHT = this->FRONT_RIGHT;

	this->FRONT_LEFT = this->LEFT;
	this->FRONT = this->MIDDLE;
	this->FRONT_RIGHT = this->RIGHT;

	this->LEFT = this->BACK_LEFT;
	this->MIDDLE = this->BACK;
	this->RIGHT = this->BACK_RIGHT;

	this->BACK_LEFT = this->CUR_FORWARD_LEFT;
	this->BACK = this->CUR_FORWARD;
	this->BACK_RIGHT = this->CUR_FORWARD_RIGHT;

	this->positionsToLoad.forwardLeft = glm::vec3(pos.x - CHUNK_X, pos.y, pos.z - CHUNK_Z);
	this->positionsToLoad.forward = glm::vec3(pos.x - CHUNK_X, pos.y, pos.z);
	this->positionsToLoad.forwardRight = glm::vec3(pos.x - CHUNK_X, pos.y, pos.z + CHUNK_Z);

	generate();
}

void TerrainLoader::shiftInPositiveZDir(glm::vec3 pos) {
	this->CUR_FORWARD_LEFT = this->FRONT_LEFT;
	this->CUR_FORWARD = this->LEFT;
	this->CUR_FORWARD_RIGHT = this->BACK_LEFT;

	this->FRONT_LEFT = this->FRONT;
	this->LEFT = this->MIDDLE;
	this->BACK_LEFT = this->BACK;

	this->FRONT = this->FRONT_RIGHT;
	this->MIDDLE = this->RIGHT;
	this->BACK = this->BACK_RIGHT;

	this->FRONT_RIGHT = this->CUR_FORWARD_LEFT;
	this->RIGHT = this->CUR_FORWARD;
	this->BACK_RIGHT = this->CUR_FORWARD_RIGHT;

	this->positionsToLoad.forwardLeft = glm::vec3(pos.x + CHUNK_X, pos.y, pos.z + CHUNK_Z);
	this->positionsToLoad.forward = glm::vec3(pos.x, pos.y, pos.z + CHUNK_Z);
	this->positionsToLoad.forwardRight = glm::vec3(pos.x - CHUNK_X, pos.y, pos.z + CHUNK_Z);

	generate();
}

void TerrainLoader::shiftInNegativeZDir(glm::vec3 pos) {
	this->CUR_FORWARD_LEFT = this->FRONT_RIGHT;
	this->CUR_FORWARD = this->RIGHT;
	this->CUR_FORWARD_RIGHT = this->BACK_RIGHT;

	this->FRONT_RIGHT = this->FRONT;
	this->RIGHT = this->MIDDLE;
	this->BACK_RIGHT = this->BACK;

	this->FRONT = this->FRONT_LEFT;
	this->MIDDLE = this->LEFT;
	this->BACK = this->BACK_LEFT;

	this->FRONT_LEFT = this->CUR_FORWARD_LEFT;
	this->LEFT = this->CUR_FORWARD;
	this->BACK_LEFT = this->CUR_FORWARD_RIGHT;

	this->positionsToLoad.forwardLeft = glm::vec3(pos.x + CHUNK_X, pos.y, pos.z - CHUNK_Z);
	this->positionsToLoad.forward = glm::vec3(pos.x, pos.y, pos.z - CHUNK_Z);
	this->positionsToLoad.forwardRight = glm::vec3(pos.x - CHUNK_X, pos.y, pos.z - CHUNK_Z);

	generate();
}

void TerrainLoader::generate() {
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->positionsToLoad.forwardLeft).getDrawablePositions()), this->CUR_FORWARD_LEFT));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->positionsToLoad.forward).getDrawablePositions()), this->CUR_FORWARD));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(this->terrainGenerator3d->generateComplex(this->positionsToLoad.forwardRight).getDrawablePositions()), this->CUR_FORWARD_RIGHT));
}
