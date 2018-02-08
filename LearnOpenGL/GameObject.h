#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "Camera.h"
#include "LightsContainer.h"

// TODO: EfficientGameObject
/* 
  Create a GameObject where every one of its components 
  has a reference back to it. Therefore if for instance, 
  the Transform of the GameObject changes it can set a variable in the 
  GameObject. Since all of the GameObject's components can set this variable 
  the GameObject can know if it has to call the Shader or not.
*/

class GameObject {
public:
	virtual void Draw() = 0;

	virtual Shader * getShader() = 0;

	virtual Camera * getCamera() = 0;

	virtual Transform * getTransform() = 0;

	virtual LightsContainer * getLightsContainer() = 0;
};
