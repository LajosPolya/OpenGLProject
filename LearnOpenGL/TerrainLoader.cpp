#include "TerrainLoader.h"

// This implementation may not work if this class was a child class
// See: https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/

TerrainLoader::TerrainLoader(Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & returnQ) : returnQ_m(returnQ_m), readyToGrab(readyToGrab), returnQ(returnQ) {
	terrainGenerator3d = new TerrainGenerator((GLint)CHUNK_X, (GLint)CHUNK_Y, (GLint)CHUNK_Z, T_3D);
	this->camera = camera;
}

TerrainLoader::~TerrainLoader() {}

// `this` pointer also needs to be sent as parameter to thread
void TerrainLoader::start() {
	t1 = std::thread(&TerrainLoader::Loader, this, camera, std::ref(returnQ_m), std::ref(readyToGrab), std::ref(returnQ));
}

void TerrainLoader::stop() {
	this->killAll = 1;
	t1.join();
}

void TerrainLoader::Loader(Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & newReturnQ) {
	GLint empty = 1;
	GLint done = 0;
	glm::vec3 prevPos = camera->Position;
	std::vector<glm::vec3> messageQ;
	GLuint messageQSize = 0;

	// Preload data so the camera can move around while this loads
	messageQ.push_back(glm::vec3(camera->Position.x - 50.0, camera->Position.y, camera->Position.z - 50.0));
	messageQ.push_back(glm::vec3(camera->Position.x - 50.0, camera->Position.y, camera->Position.z + 50.0));
	messageQ.push_back(glm::vec3(camera->Position.x + 50.0, camera->Position.y, camera->Position.z + 50.0));
	messageQ.push_back(glm::vec3(camera->Position.x - 50.0, camera->Position.y, camera->Position.z));
	messageQ.push_back(glm::vec3(camera->Position.x, camera->Position.y, camera->Position.z + 50.0));
	messageQ.push_back(glm::vec3(camera->Position.x, camera->Position.y, camera->Position.z - 50.0));
	messageQ.push_back(glm::vec3(camera->Position.x + 50.0, camera->Position.y, camera->Position.z - 50.0));
	messageQ.push_back(glm::vec3(camera->Position.x + 50.0, camera->Position.y, camera->Position.z));
	messageQ.push_back(glm::vec3(camera->Position.x, camera->Position.y, camera->Position.z));

	std::cout << "Starting Init Generation" << std::endl;
	messageQSize = messageQ.size();
	for (GLuint i = 0; i < messageQSize; i++) {
		std::cout << i << std::endl;
		glm::vec3 initPos = messageQ[messageQ.size() - 1];
		messageQ.pop_back();
		terrainGenerator3d->generateComplex(initPos.x, initPos.y, initPos.z);
	}
	std::cout << "Ending Init Generation" << std::endl;
	std::cout << camera->Position.x << " " << camera->Position.y << " " << camera->Position.z << std::endl;

	returnQ_m.lock();
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(camera->Position.x - 50, camera->Position.y, camera->Position.z + 50).getDrawablePositions()), 8));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(camera->Position.x - 50, camera->Position.y, camera->Position.z).getDrawablePositions()), 7));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(camera->Position.x - 50, camera->Position.y, camera->Position.z - 50).getDrawablePositions()), 6));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(camera->Position.x, camera->Position.y, camera->Position.z + 50).getDrawablePositions()), 5));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(camera->Position.x, camera->Position.y, camera->Position.z).getDrawablePositions()), 4));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(camera->Position.x, camera->Position.y, camera->Position.z - 50).getDrawablePositions()), 3));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(camera->Position.x + 50, camera->Position.y, camera->Position.z + 50).getDrawablePositions()), 2));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(camera->Position.x + 50, camera->Position.y, camera->Position.z).getDrawablePositions()), 1));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(camera->Position.x + 50, camera->Position.y, camera->Position.z - 50).getDrawablePositions()), 0));
	returnQ_m.unlock();
	readyToGrab = 1;
	while (this->killAll != 1) {
		glm::vec3 pos = camera->Position;
		glm::vec3 chunkPos = terrainGenerator3d->getChunkPos(camera->Position);
		// Let's assume positive x is forward
		if (terrainGenerator3d->shouldGetNewChunks(pos) == 1 && readyToGrab == 0) {
			std::cout << "  BEFORE  " << pos.x << " " << pos.z << " Chunk: " << chunkPos.x << " " << chunkPos.z << std::endl;
			std::cout << this->FRONT_LEFT << " " << this->FRONT << " " << this->FRONT_RIGHT << std::endl;
			std::cout << this->LEFT << " " << this->MIDDLE << " " << this->RIGHT << std::endl;
			std::cout << this->BACK_LEFT << " " << this->BACK << " " << this->BACK_RIGHT << std::endl;
			if (chunkPos.x > prevPos.x) {
				returnQ_m.lock();
				shiftInPositiveXDir(pos);
				returnQ_m.unlock();
				readyToGrab = 1;
			}
			else if (chunkPos.x < prevPos.x) {
				returnQ_m.lock();
				shiftInNegativeXDir(pos);
				returnQ_m.unlock();
				readyToGrab = 1;
			}
			else if (chunkPos.z > prevPos.z) {
				returnQ_m.lock();
				shiftInPositiveZDir(pos);
				returnQ_m.unlock();
				readyToGrab = 1;
			}
			else if (chunkPos.z < prevPos.z) {
				returnQ_m.lock();
				shiftInNegativeZDir(pos);
				returnQ_m.unlock();
				readyToGrab = 1;
			}
			std::cout << "  AFTER  " << std::endl;
			std::cout << this->FRONT_LEFT << " " << this->FRONT << " " << this->FRONT_RIGHT << std::endl;
			std::cout << this->LEFT << " " << this->MIDDLE << " " << this->RIGHT << std::endl;
			std::cout << this->BACK_LEFT << " " << this->BACK << " " << this->BACK_RIGHT << std::endl;
			std::cout << std::endl << std::endl;
			prevPos = chunkPos;
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

	
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x + CHUNK_X, pos.y, pos.z - CHUNK_Z).getDrawablePositions()), this->FRONT_LEFT));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x + CHUNK_X, pos.y, pos.z).getDrawablePositions()), this->FRONT));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x + CHUNK_X, pos.y, pos.z + CHUNK_Z).getDrawablePositions()), this->FRONT_RIGHT));
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

	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x - CHUNK_X, pos.y, pos.z - CHUNK_Z).getDrawablePositions()), this->BACK_LEFT));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x - CHUNK_X, pos.y, pos.z).getDrawablePositions()), this->BACK));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x - CHUNK_X, pos.y, pos.z + CHUNK_Z).getDrawablePositions()), this->BACK_RIGHT));

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

	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x + CHUNK_X, pos.y, pos.z + CHUNK_Z).getDrawablePositions()), this->FRONT_RIGHT));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x, pos.y, pos.z + CHUNK_Z).getDrawablePositions()), this->RIGHT));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x - CHUNK_X, pos.y, pos.z + CHUNK_Z).getDrawablePositions()), this->BACK_RIGHT));
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

	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x + CHUNK_X, pos.y, pos.z - CHUNK_Z).getDrawablePositions()), this->FRONT_LEFT));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x, pos.y, pos.z - CHUNK_Z).getDrawablePositions()), this->LEFT));
	this->returnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d->generateComplex(pos.x - CHUNK_X, pos.y, pos.z - CHUNK_Z).getDrawablePositions()), this->BACK_LEFT));
}
