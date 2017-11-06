#pragma once

#include "Camera.h"
#include "TransformImpl.h"
#include "InstancedTransformImpl.h"
#include "InstancedArrayTransformImpl.h"

class CollisionDetector
{
public:
	static void addCamera(Camera * camera);

	static void AddTransform(Transform * transform);
	static void AddTransform(InstancedTransformImpl * transform);
	static void AddTransform(InstancedArrayTransformImpl * transform);

	static void CheckCollisions();

	static void SetPrevPosiion(glm::vec3 position);
private:
	static Camera * camera;

	static glm::vec3 prevPosition;

	static std::vector<Transform*> transforms;
	static std::vector<InstancedTransformImpl*> instancedTransforms;
	static std::vector<InstancedArrayTransformImpl*> instancedArrayTransforms;

	static void CheckCollision(Transform & transform);
	static void CheckCollision(InstancedTransformImpl & transform);
	static void CheckCollision(InstancedArrayTransformImpl & transform);
};

