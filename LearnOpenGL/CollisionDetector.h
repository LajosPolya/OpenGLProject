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

	static void SetPrevPosiion(vec3 position);
private:
	static Camera * camera;

	static vec3 prevPosition;

	static vector<Transform*> transforms;
	static vector<InstancedTransformImpl*> instancedTransforms;
	static vector<InstancedArrayTransformImpl*> instancedArrayTransforms;

	static void CheckCollision(Transform & transform);
	static void CheckCollision(InstancedTransformImpl & transform);
	static void CheckCollision(InstancedArrayTransformImpl & transform);

	static const GLfloat padding;
};

