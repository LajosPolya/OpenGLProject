#pragma once

#include <vector>
#include <string>

#include "TransparentTransformImpl.h"
#include "GameObjectUtils.h"
#include "GameObjectMemoryManager.h"
#include "Camera.h"
#include "Shader.h"

using namespace std;
using namespace glm;

class TransparentGameObjectImpl
{
public:
	TransparentGameObjectImpl() = default;
	~TransparentGameObjectImpl() = default;

	TransparentGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, string diffuseMapLoc, string specularMapLoc, string meshLoc,  GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, const Camera & camera, mat4 projection);
	TransparentGameObjectImpl(string vertexShader, string fragmentShader, GLchar * diffuseMapLoc, GLchar * meshLoc, string transformLoc, const Camera & camera, mat4 projection);

	void Draw();

	Shader * getShader();

	Camera getCamera();

	TransparentTransformImpl * getTransform();

	LightsContainer * getLightsContainer();

private:
	vector<InstancedMesh*> mesh;

	Material * material;

	Shader * shader;

	const Camera & camera;

	TransparentTransformImpl * transform;

	LightsContainer * lightsContainer;

	mat4 projection;

	vector<InstancedMesh*> makeMeshes(string path, GLuint numInstances, vector<Texture*> diffuseMaps, vector<Texture*> specularMaps);
};

