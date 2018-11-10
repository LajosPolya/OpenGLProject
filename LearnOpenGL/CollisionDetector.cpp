#include "CollisionDetector.h"

Camera * CollisionDetector::camera;

vec3 CollisionDetector::prevPosition;

vector<Transform*> CollisionDetector::transforms;
vector<InstancedTransformImpl*> CollisionDetector::instancedTransforms;
vector<InstancedArrayTransformImpl*> CollisionDetector::instancedArrayTransforms;

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
	prevPosition = camera->Position;
}

void CollisionDetector::AddTransform(Transform * transform) {
	transforms.push_back(transform);
}

void CollisionDetector::AddTransform(InstancedTransformImpl * transform) {
	instancedTransforms.push_back(transform);
}

void CollisionDetector::AddTransform(InstancedArrayTransformImpl * transform) {
	instancedArrayTransforms.push_back(transform);
}

void CollisionDetector::CheckCollision(Transform & transform) {
	vec3 Position = transform.getPosition();
	if (Position.x - padding< camera->Position.x && Position.x + padding> camera->Position.x
		&& Position.y - padding< camera->Position.y && Position.y + padding> camera->Position.y
		&& Position.z - padding< camera->Position.z && Position.z + padding> camera->Position.z) {

		if (Position.x - padding< camera->Position.x && Position.x + padding> camera->Position.x
			&& Position.y - padding< prevPosition.y && Position.y + padding> prevPosition.y
			&& Position.z - padding< prevPosition.z && Position.z + padding> prevPosition.z) {

			camera->Position.x = prevPosition.x;
		}

		if (Position.x - padding< prevPosition.x && Position.x + padding> prevPosition.x
			&& Position.y - padding< prevPosition.y && Position.y + padding> prevPosition.y
			&& Position.z - padding< camera->Position.z && Position.z + padding> camera->Position.z) {

			camera->Position.z = prevPosition.z;
		}

		if (Position.x - padding< prevPosition.x && Position.x + padding> prevPosition.x
			&& Position.y - padding< camera->Position.y && Position.y + padding> camera->Position.y
			&& Position.z - padding< prevPosition.z && Position.z + padding> prevPosition.z) {

			camera->Position.y = prevPosition.y;
		}
	}
}

void CollisionDetector::CheckCollision(InstancedTransformImpl & transform) {
	GLuint i;

	vector<vec3> * Positions = transform.getPositions();
	GLuint size = Positions->size();
	for (i = 0; i < size; i++) {
		if ((*Positions)[i].x - padding< camera->Position.x && (*Positions)[i].x + padding> camera->Position.x
			&& (*Positions)[i].y - padding< camera->Position.y && (*Positions)[i].y + padding> camera->Position.y
			&& (*Positions)[i].z - padding< camera->Position.z && (*Positions)[i].z + padding> camera->Position.z) {

			if ((*Positions)[i].x - padding< camera->Position.x && (*Positions)[i].x + padding> camera->Position.x
				&& (*Positions)[i].y - padding< prevPosition.y && (*Positions)[i].y + padding> prevPosition.y
				&& (*Positions)[i].z - padding< prevPosition.z && (*Positions)[i].z + padding> prevPosition.z) {

				camera->Position.x = prevPosition.x;
			}

			if ((*Positions)[i].x - padding< prevPosition.x && (*Positions)[i].x + padding> prevPosition.x
				&& (*Positions)[i].y - padding< prevPosition.y && (*Positions)[i].y + padding> prevPosition.y
				&& (*Positions)[i].z - padding< camera->Position.z && (*Positions)[i].z + padding> camera->Position.z) {

				camera->Position.z = prevPosition.z;
			}

			if ((*Positions)[i].x - padding< prevPosition.x && (*Positions)[i].x + padding> prevPosition.x
				&& (*Positions)[i].y - padding< camera->Position.y && (*Positions)[i].y + padding> camera->Position.y
				&& (*Positions)[i].z - padding< prevPosition.z && (*Positions)[i].z + padding> prevPosition.z) {

				camera->Position.y = prevPosition.y;
			}
		}
	}
}

void CollisionDetector::CheckCollision(InstancedArrayTransformImpl & transform) {
	GLuint i;

	vector<vec3> * Positions = transform.getPositions();
	GLuint size = Positions->size();
	for (i = 0; i < size; i++) {
		if ((*Positions)[i].x - padding< camera->Position.x && (*Positions)[i].x + padding> camera->Position.x
			&& (*Positions)[i].y - padding< camera->Position.y && (*Positions)[i].y + padding> camera->Position.y
			&& (*Positions)[i].z - padding< camera->Position.z && (*Positions)[i].z + padding> camera->Position.z) {

			if ((*Positions)[i].x - padding< camera->Position.x && (*Positions)[i].x + padding> camera->Position.x
				&& (*Positions)[i].y - padding< prevPosition.y && (*Positions)[i].y + padding> prevPosition.y
				&& (*Positions)[i].z - padding< prevPosition.z && (*Positions)[i].z + padding> prevPosition.z) {

				camera->Position.x = prevPosition.x;
			}

			if ((*Positions)[i].x - padding< prevPosition.x && (*Positions)[i].x + padding> prevPosition.x
				&& (*Positions)[i].y - padding< prevPosition.y && (*Positions)[i].y + padding> prevPosition.y
				&& (*Positions)[i].z - padding< camera->Position.z && (*Positions)[i].z + padding> camera->Position.z) {

				camera->Position.z = prevPosition.z;
			}

			if ((*Positions)[i].x - padding< prevPosition.x && (*Positions)[i].x + padding> prevPosition.x
				&& (*Positions)[i].y - padding< camera->Position.y && (*Positions)[i].y + padding> camera->Position.y
				&& (*Positions)[i].z - padding< prevPosition.z && (*Positions)[i].z + padding> prevPosition.z) {

				camera->Position.y = prevPosition.y;
			}
		}
	}
}
