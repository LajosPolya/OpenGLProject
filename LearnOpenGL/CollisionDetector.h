#pragma once

#include "Camera.h"
#include "TransformImpl.h"
#include "InstancedTransformImpl.h"
#include "InstancedArrayTransformImpl.h"

class CollisionDetector
{
public:
	static void addCamera(Camera * camera);

	static void AddTransform(const Transform * transform);
	static void AddTransform(const InstancedTransformImpl * transform);
	static void AddTransform(const InstancedArrayTransformImpl * transform);

	static void CheckCollisions();

	static void SetPrevPosiion(vec3 position);
private:
	static Camera * camera;

	static vec3 prevPosition;

	static vector<const Transform*> transforms;
	static vector<const InstancedTransformImpl*> instancedTransforms;
	static vector<const InstancedArrayTransformImpl*> instancedArrayTransforms;

	static void CheckCollision(const Transform & transform);
	static void CheckCollision(const InstancedTransformImpl & transform);
	static void CheckCollision(const InstancedArrayTransformImpl & transform);

	static const GLfloat padding;
};

