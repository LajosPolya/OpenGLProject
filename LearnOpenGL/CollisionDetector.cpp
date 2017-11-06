#include "CollisionDetector.h"

Camera * CollisionDetector::camera;

glm::vec3 CollisionDetector::prevPosition;

std::vector<Transform*> CollisionDetector::transforms;
std::vector<InstancedTransformImpl*> CollisionDetector::instancedTransforms;
std::vector<InstancedArrayTransformImpl*> CollisionDetector::instancedArrayTransforms;


void CollisionDetector::CheckCollisions()
{
	GLuint i;
	for (i = 0; i < transforms.size(); i++) {
		CheckCollision(*(transforms[i]));
	}

	for (i = 0; i < instancedTransforms.size(); i++) {
		CheckCollision(*(instancedTransforms[i]));
	}

	for (i = 0; i < instancedArrayTransforms.size(); i++) {
		CheckCollision(*(instancedArrayTransforms[i]));
	}
}

void CollisionDetector::SetPrevPosiion(glm::vec3 position)
{
	prevPosition = position;
}

void CollisionDetector::addCamera(Camera * camera)
{
	CollisionDetector::camera = camera;
	prevPosition = camera->Position;
}

void CollisionDetector::AddTransform(Transform * transform)
{
	transforms.push_back(transform);
}

void CollisionDetector::AddTransform(InstancedTransformImpl * transform)
{
	instancedTransforms.push_back(transform);
}

void CollisionDetector::AddTransform(InstancedArrayTransformImpl * transform)
{
	instancedArrayTransforms.push_back(transform);
}

void CollisionDetector::CheckCollision(Transform & transform)
{
	glm::vec3 Position = transform.getPosition();
	if (Position.x - 0.8< camera->Position.x && Position.x + 0.8> camera->Position.x
		&& Position.y - 0.8< camera->Position.y && Position.y + 0.8> camera->Position.y
		&& Position.z - 0.8< camera->Position.z && Position.z + 0.8> camera->Position.z) {

		if (Position.x - 0.8< camera->Position.x && Position.x + 0.8> camera->Position.x
			&& Position.y - 0.8< prevPosition.y && Position.y + 0.8> prevPosition.y
			&& Position.z - 0.8< prevPosition.z && Position.z + 0.8> prevPosition.z) {

			camera->Position.x = prevPosition.x;
		}

		if (Position.x - 0.8< prevPosition.x && Position.x + 0.8> prevPosition.x
			&& Position.y - 0.8< prevPosition.y && Position.y + 0.8> prevPosition.y
			&& Position.z - 0.8< camera->Position.z && Position.z + 0.8> camera->Position.z) {

			camera->Position.z = prevPosition.z;
		}

		if (Position.x - 0.8< prevPosition.x && Position.x + 0.8> prevPosition.x
			&& Position.y - 0.8< camera->Position.y && Position.y + 0.8> camera->Position.y
			&& Position.z - 0.8< prevPosition.z && Position.z + 0.8> prevPosition.z) {

			camera->Position.y = prevPosition.y;
		}
	}
}

void CollisionDetector::CheckCollision(InstancedTransformImpl & transform)
{
	GLuint i;

	std::vector<glm::vec3> * Positions = transform.getPositions();
	GLuint size = Positions->size();
	for (i = 0; i < size; i++) {
		if ((*Positions)[i].x - 0.8< camera->Position.x && (*Positions)[i].x + 0.8> camera->Position.x
			&& (*Positions)[i].y - 0.8< camera->Position.y && (*Positions)[i].y + 0.8> camera->Position.y
			&& (*Positions)[i].z - 0.8< camera->Position.z && (*Positions)[i].z + 0.8> camera->Position.z) {

			if ((*Positions)[i].x - 0.8< camera->Position.x && (*Positions)[i].x + 0.8> camera->Position.x
				&& (*Positions)[i].y - 0.8< prevPosition.y && (*Positions)[i].y + 0.8> prevPosition.y
				&& (*Positions)[i].z - 0.8< prevPosition.z && (*Positions)[i].z + 0.8> prevPosition.z) {

				camera->Position.x = prevPosition.x;
			}

			if ((*Positions)[i].x - 0.8< prevPosition.x && (*Positions)[i].x + 0.8> prevPosition.x
				&& (*Positions)[i].y - 0.8< prevPosition.y && (*Positions)[i].y + 0.8> prevPosition.y
				&& (*Positions)[i].z - 0.8< camera->Position.z && (*Positions)[i].z + 0.8> camera->Position.z) {

				camera->Position.z = prevPosition.z;
			}

			if ((*Positions)[i].x - 0.8< prevPosition.x && (*Positions)[i].x + 0.8> prevPosition.x
				&& (*Positions)[i].y - 0.8< camera->Position.y && (*Positions)[i].y + 0.8> camera->Position.y
				&& (*Positions)[i].z - 0.8< prevPosition.z && (*Positions)[i].z + 0.8> prevPosition.z) {

				camera->Position.y = prevPosition.y;
			}
		}
	}
}

void CollisionDetector::CheckCollision(InstancedArrayTransformImpl & transform)
{
	GLuint i;

	std::vector<glm::vec3> * Positions = transform.getPositions();
	GLuint size = Positions->size();
	for (i = 0; i < size; i++) {
		if ((*Positions)[i].x - 0.8< camera->Position.x && (*Positions)[i].x + 0.8> camera->Position.x
			&& (*Positions)[i].y - 0.8< camera->Position.y && (*Positions)[i].y + 0.8> camera->Position.y
			&& (*Positions)[i].z - 0.8< camera->Position.z && (*Positions)[i].z + 0.8> camera->Position.z) {

			if ((*Positions)[i].x - 0.8< camera->Position.x && (*Positions)[i].x + 0.8> camera->Position.x
				&& (*Positions)[i].y - 0.8< prevPosition.y && (*Positions)[i].y + 0.8> prevPosition.y
				&& (*Positions)[i].z - 0.8< prevPosition.z && (*Positions)[i].z + 0.8> prevPosition.z) {

				camera->Position.x = prevPosition.x;
			}

			if ((*Positions)[i].x - 0.8< prevPosition.x && (*Positions)[i].x + 0.8> prevPosition.x
				&& (*Positions)[i].y - 0.8< prevPosition.y && (*Positions)[i].y + 0.8> prevPosition.y
				&& (*Positions)[i].z - 0.8< camera->Position.z && (*Positions)[i].z + 0.8> camera->Position.z) {

				camera->Position.z = prevPosition.z;
			}

			if ((*Positions)[i].x - 0.8< prevPosition.x && (*Positions)[i].x + 0.8> prevPosition.x
				&& (*Positions)[i].y - 0.8< camera->Position.y && (*Positions)[i].y + 0.8> camera->Position.y
				&& (*Positions)[i].z - 0.8< prevPosition.z && (*Positions)[i].z + 0.8> prevPosition.z) {

				camera->Position.y = prevPosition.y;
			}
		}
	}
}
