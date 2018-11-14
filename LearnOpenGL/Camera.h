#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 10.0f;
const GLfloat SENSITIVITY = 0.5f;
const GLfloat ZOOM = 45.0f;

class Camera {
public:
	// Constructor with vectors
	Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);


	mat4 GetViewMatrix();

	const mat4 GetViewMatrix() const;

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

	void ProcessMouseScroll(GLfloat yoffset);

	const vec3 & getPosition() const;

	vec3 & getPosition();

	const vec3 & getFront() const;

	const GLfloat & getZoom() const;

private:
	// Camera Attributes
	vec3 Position;
	vec3 Front;
	vec3 Up;
	vec3 Right;
	vec3 WorldUp;

	// Euler Angles
	GLfloat Yaw;
	GLfloat Pitch;

	// Camera Positions
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	void updateCameraVectors();
};