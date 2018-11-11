#include "CollisionDetector.h"

Camera * CollisionDetector::camera;

vec3 CollisionDetector::prevPosition;

vector<const Transform*> CollisionDetector::transforms;
vector<const InstancedTransformImpl*> CollisionDetector::instancedTransforms;
vector<const InstancedArrayTransformImpl*> CollisionDetector::instancedArrayTransforms;

const GLfloat CollisionDetector::padding = 0.8f;


void CollisionDetector::CheckCollisions() {
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

void CollisionDetector::SetPrevPosiion(vec3 position) {
	prevPosition = position;
}

void CollisionDetector::addCamera(Camera * camera) {
	CollisionDetector::camera = camera;
	prevPosition = camera->getPosition();
}

void CollisionDetector::AddTransform(const Transform * transform) {
	transforms.push_back(transform);
}

void CollisionDetector::AddTransform(const InstancedTransformImpl * transform) {
	instancedTransforms.push_back(transform);
}

void CollisionDetector::AddTransform(const InstancedArrayTransformImpl * transform) {
	instancedArrayTransforms.push_back(transform);
}

void CollisionDetector::CheckCollision(const Transform & transform) {
	const vec3 Position = transform.getPosition();
	const vec3 cPos = camera->getPosition();

	if (Position.x - padding< cPos.x && Position.x + padding> cPos.x
		&& Position.y - padding< cPos.y && Position.y + padding> cPos.y
		&& Position.z - padding< cPos.z && Position.z + padding> cPos.z) {

		if (Position.x - padding< cPos.x && Position.x + padding> cPos.x
			&& Position.y - padding< prevPosition.y && Position.y + padding> prevPosition.y
			&& Position.z - padding< prevPosition.z && Position.z + padding> prevPosition.z) {

			camera->getPosition().x = prevPosition.x;
		}

		if (Position.x - padding< prevPosition.x && Position.x + padding> prevPosition.x
			&& Position.y - padding< prevPosition.y && Position.y + padding> prevPosition.y
			&& Position.z - padding< cPos.z && Position.z + padding> cPos.z) {

			camera->getPosition().z = prevPosition.z;
		}

		if (Position.x - padding< prevPosition.x && Position.x + padding> prevPosition.x
			&& Position.y - padding< cPos.y && Position.y + padding> cPos.y
			&& Position.z - padding< prevPosition.z && Position.z + padding> prevPosition.z) {

			camera->getPosition().y = prevPosition.y;
		}
	}
}

void CollisionDetector::CheckCollision(const InstancedTransformImpl & transform) {
	GLuint i;

	const vector<vec3> * Positions = transform.getPositions();
	const vec3 cPos = camera->getPosition();
	GLuint size = Positions->size();

	for (i = 0; i < size; i++) {
		if ((*Positions)[i].x - padding< cPos.x && (*Positions)[i].x + padding> cPos.x
			&& (*Positions)[i].y - padding< cPos.y && (*Positions)[i].y + padding> cPos.y
			&& (*Positions)[i].z - padding< cPos.z && (*Positions)[i].z + padding> cPos.z) {

			if ((*Positions)[i].x - padding< cPos.x && (*Positions)[i].x + padding> cPos.x
				&& (*Positions)[i].y - padding< prevPosition.y && (*Positions)[i].y + padding> prevPosition.y
				&& (*Positions)[i].z - padding< prevPosition.z && (*Positions)[i].z + padding> prevPosition.z) {

				camera->getPosition().x = prevPosition.x;
			}

			if ((*Positions)[i].x - padding< prevPosition.x && (*Positions)[i].x + padding> prevPosition.x
				&& (*Positions)[i].y - padding< prevPosition.y && (*Positions)[i].y + padding> prevPosition.y
				&& (*Positions)[i].z - padding< cPos.z && (*Positions)[i].z + padding> cPos.z) {

				camera->getPosition().z = prevPosition.z;
			}

			if ((*Positions)[i].x - padding< prevPosition.x && (*Positions)[i].x + padding> prevPosition.x
				&& (*Positions)[i].y - padding< cPos.y && (*Positions)[i].y + padding> cPos.y
				&& (*Positions)[i].z - padding< prevPosition.z && (*Positions)[i].z + padding> prevPosition.z) {

				camera->getPosition().y = prevPosition.y;
			}
		}
	}
}

void CollisionDetector::CheckCollision(const InstancedArrayTransformImpl & transform) {
	GLuint i;

	const vector<vec3> * Positions = transform.getPositions();
	const vec3 cPos = camera->getPosition();
	GLuint size = Positions->size();

	for (i = 0; i < size; i++) {
		if ((*Positions)[i].x - padding< cPos.x && (*Positions)[i].x + padding> cPos.x
			&& (*Positions)[i].y - padding< cPos.y && (*Positions)[i].y + padding> cPos.y
			&& (*Positions)[i].z - padding< cPos.z && (*Positions)[i].z + padding> cPos.z) {

			if ((*Positions)[i].x - padding< cPos.x && (*Positions)[i].x + padding> cPos.x
				&& (*Positions)[i].y - padding< prevPosition.y && (*Positions)[i].y + padding> prevPosition.y
				&& (*Positions)[i].z - padding< prevPosition.z && (*Positions)[i].z + padding> prevPosition.z) {

				camera->getPosition().x = prevPosition.x;
			}

			if ((*Positions)[i].x - padding< prevPosition.x && (*Positions)[i].x + padding> prevPosition.x
				&& (*Positions)[i].y - padding< prevPosition.y && (*Positions)[i].y + padding> prevPosition.y
				&& (*Positions)[i].z - padding< cPos.z && (*Positions)[i].z + padding> cPos.z) {

				camera->getPosition().z = prevPosition.z;
			}

			if ((*Positions)[i].x - padding< prevPosition.x && (*Positions)[i].x + padding> prevPosition.x
				&& (*Positions)[i].y - padding< cPos.y && (*Positions)[i].y + padding> cPos.y
				&& (*Positions)[i].z - padding< prevPosition.z && (*Positions)[i].z + padding> prevPosition.z) {

				camera->getPosition().y = prevPosition.y;
			}
		}
	}
}
