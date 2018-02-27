#include "TerrainLoader.h"

// This implementation may not work if this class was a child class
// See: https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/

TerrainLoader::TerrainLoader(GLint chunkX, GLint chunkY, GLint chunkZ) {
	this->CHUNK_X = chunkX;
	this->CHUNK_Y = chunkY;
	this->CHUNK_Z = chunkZ;
}

TerrainLoader::~TerrainLoader() {}

// `this` pointer also needs to be sent as parameter to thread
void TerrainLoader::start(TerrainGenerator & terrainGenerator3d, Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & returnQ) {
	t1 = std::thread(&TerrainLoader::Loader, this, std::ref(terrainGenerator3d), camera, std::ref(returnQ_m), std::ref(readyToGrab), std::ref(returnQ));
}

void TerrainLoader::stop() {
	this->killAll = 1;
	t1.join();
}

void TerrainLoader::Loader(TerrainGenerator & terrainGenerator3d, Camera * camera, std::mutex & returnQ_m, GLuint & readyToGrab, std::vector<PositionRelativeCamera> & newReturnQ) {
	// These consts represent a 3x3 grid viewed as a 1d array (the array storing all the GameObject in the main loop)
	GLint FRONT_LEFT = 0;
	GLint FRONT = 1;
	GLint FRONT_RIGHT = 2;
	GLint LEFT = 3;
	GLint MIDDLE = 4;
	GLint RIGHT = 5;
	GLint BACK_LEFT = 6;
	GLint BACK = 7;
	GLint BACK_RIGHT = 8;
	GLint empty = 1;
	GLint done = 0;
	glm::vec3 prevPos = camera->Position;
	std::vector<glm::vec3> messageQ;
	GLuint messageQSize = 0;

	messageQ.push_back(glm::vec3(camera->Position.x - 50.0, camera->Position.y, camera->Position.z + 100.0));
	messageQ.push_back(glm::vec3(camera->Position.x, camera->Position.y, camera->Position.z + 100.0));
	messageQ.push_back(glm::vec3(camera->Position.x + 50.0, camera->Position.y, camera->Position.z + 100.0));

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
		terrainGenerator3d.generateComplex(initPos.x, initPos.y, initPos.z);
	}
	std::cout << "Ending Init Generation" << std::endl;
	std::cout << camera->Position.x << " " << camera->Position.y << " " << camera->Position.z << std::endl;

	returnQ_m.lock();
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(camera->Position.x - 50, camera->Position.y, camera->Position.z + 50).getDrawablePositions()), 8));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(camera->Position.x - 50, camera->Position.y, camera->Position.z).getDrawablePositions()), 7));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(camera->Position.x - 50, camera->Position.y, camera->Position.z - 50).getDrawablePositions()), 6));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(camera->Position.x, camera->Position.y, camera->Position.z + 50).getDrawablePositions()), 5));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(camera->Position.x, camera->Position.y, camera->Position.z).getDrawablePositions()), 4));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(camera->Position.x, camera->Position.y, camera->Position.z - 50).getDrawablePositions()), 3));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(camera->Position.x + 50, camera->Position.y, camera->Position.z + 50).getDrawablePositions()), 2));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(camera->Position.x + 50, camera->Position.y, camera->Position.z).getDrawablePositions()), 1));
	newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(camera->Position.x + 50, camera->Position.y, camera->Position.z - 50).getDrawablePositions()), 0));
	returnQ_m.unlock();
	readyToGrab = 1;
	while (this->killAll != 1) {
		glm::vec3 pos = camera->Position;
		glm::vec3 chunkPos = terrainGenerator3d.getChunkPos(camera->Position);
		// Let's assume positive x is forward
		if (terrainGenerator3d.shouldGetNewChunks(pos) == 1 && readyToGrab == 0) {
			std::cout << "  BEFORE  " << pos.x << " " << pos.z << " Chunk: " << chunkPos.x << " " << chunkPos.z << std::endl;
			std::cout << FRONT_LEFT << " " << FRONT << " " << FRONT_RIGHT << std::endl;
			std::cout << LEFT << " " << MIDDLE << " " << RIGHT << std::endl;
			std::cout << BACK_LEFT << " " << BACK << " " << BACK_RIGHT << std::endl;
			if (chunkPos.x > prevPos.x) {
				GLint tempBackLeft = BACK_LEFT;
				GLint tempBack = BACK;
				GLint tempBackRight = BACK_RIGHT;

				BACK_LEFT = LEFT;
				BACK = MIDDLE;
				BACK_RIGHT = RIGHT;

				LEFT = FRONT_LEFT;
				MIDDLE = FRONT;
				RIGHT = FRONT_RIGHT;

				FRONT_LEFT = tempBackLeft;
				FRONT = tempBack;
				FRONT_RIGHT = tempBackRight;

				returnQ_m.lock();
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x + CHUNK_X, pos.y, pos.z).getDrawablePositions()), FRONT));
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x + CHUNK_X, pos.y, pos.z + CHUNK_Z).getDrawablePositions()), FRONT_RIGHT));
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x + CHUNK_X, pos.y, pos.z - CHUNK_Z).getDrawablePositions()), FRONT_LEFT));
				returnQ_m.unlock();
				readyToGrab = 1;
			}
			else if (chunkPos.x < prevPos.x) {
				GLint tempFrontLeft = FRONT_LEFT;
				GLint tempFront = FRONT;
				GLint tempFrontRight = FRONT_RIGHT;

				FRONT_LEFT = LEFT;
				FRONT = MIDDLE;
				FRONT_RIGHT = RIGHT;

				LEFT = BACK_LEFT;
				MIDDLE = BACK;
				RIGHT = BACK_RIGHT;

				BACK_LEFT = tempFrontLeft;
				BACK = tempFront;
				BACK_RIGHT = tempFrontRight;

				returnQ_m.lock();
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x - CHUNK_X, pos.y, pos.z).getDrawablePositions()), BACK));
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x - CHUNK_X, pos.y, pos.z + CHUNK_Z).getDrawablePositions()), BACK_RIGHT));
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x - CHUNK_X, pos.y, pos.z - CHUNK_Z).getDrawablePositions()), BACK_LEFT));
				returnQ_m.unlock();
				readyToGrab = 1;
			}
			else if (chunkPos.z > prevPos.z) {
				GLint tempFrontLeft = FRONT_LEFT;
				GLint tempLeft = LEFT;
				GLint tempBackLeft = BACK_LEFT;

				FRONT_LEFT = FRONT;
				LEFT = MIDDLE;
				BACK_LEFT = BACK;

				FRONT = FRONT_RIGHT;
				MIDDLE = RIGHT;
				BACK = BACK_RIGHT;

				FRONT_RIGHT = tempFrontLeft;
				RIGHT = tempLeft;
				BACK_RIGHT = tempBackLeft;

				returnQ_m.lock();
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x + CHUNK_X, pos.y, pos.z + CHUNK_Z).getDrawablePositions()), FRONT_RIGHT));
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x, pos.y, pos.z + CHUNK_Z).getDrawablePositions()), RIGHT));
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x - CHUNK_X, pos.y, pos.z + CHUNK_Z).getDrawablePositions()), BACK_RIGHT));
				returnQ_m.unlock();
				readyToGrab = 1;
			}
			else if (chunkPos.z < prevPos.z) {
				GLint tempFrontRight = FRONT_RIGHT;
				GLint tempRight = RIGHT;
				GLint tempBackRight = BACK_RIGHT;

				FRONT_RIGHT = FRONT;
				RIGHT = MIDDLE;
				BACK_RIGHT = BACK;

				FRONT = FRONT_LEFT;
				MIDDLE = LEFT;
				BACK = BACK_LEFT;

				FRONT_LEFT = tempFrontRight;
				LEFT = tempRight;
				BACK_LEFT = tempBackRight;

				returnQ_m.lock();
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x + CHUNK_X, pos.y, pos.z - CHUNK_Z).getDrawablePositions()), FRONT_LEFT));
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x, pos.y, pos.z - CHUNK_Z).getDrawablePositions()), LEFT));
				newReturnQ.push_back(PositionRelativeCamera(InstancedArrayTransformImpl(terrainGenerator3d.generateComplex(pos.x - CHUNK_X, pos.y, pos.z - CHUNK_Z).getDrawablePositions()), BACK_LEFT));
				returnQ_m.unlock();
				readyToGrab = 1;
			}
			std::cout << "  AFTER  " << std::endl;
			std::cout << FRONT_LEFT << " " << FRONT << " " << FRONT_RIGHT << std::endl;
			std::cout << LEFT << " " << MIDDLE << " " << RIGHT << std::endl;
			std::cout << BACK_LEFT << " " << BACK << " " << BACK_RIGHT << std::endl;
			std::cout << std::endl << std::endl;
			prevPos = chunkPos;
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
}
