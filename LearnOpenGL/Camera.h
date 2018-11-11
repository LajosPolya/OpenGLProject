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
	Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(vec3(1.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		this->Position = vec3(posX, posY, posZ);
		this->WorldUp = vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}


	mat4 GetViewMatrix() {
		return lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	const mat4 GetViewMatrix() const {
		return lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD) {
			this->Position += this->Front * velocity;
		}
		if (direction == BACKWARD) {
			this->Position -= this->Front * velocity;
		}	
		if (direction == LEFT) {
			this->Position -= this->Right * velocity;
		}
		if (direction == RIGHT) {
			this->Position += this->Right * velocity;
		}
	}

	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch -= yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;
	}

	const vec3 & getPosition() const {
		return this->Position;
	}

	vec3 & getPosition() {
		return this->Position;
	}

	const vec3 & getFront() const {
		return this->Front;
	}

	const GLfloat & getZoom() const {
		return this->Zoom;
	}

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

	void updateCameraVectors()
	{
		// Calculate the new Front vector
		vec3 front;
		front.x = cos(radians(this->Yaw)) * cos(radians(this->Pitch));
		front.y = sin(radians(this->Pitch));
		front.z = sin(radians(this->Yaw)) * cos(radians(this->Pitch));
		this->Front = normalize(front);
		// Also re-calculate the Right and Up vector
		this->Right = normalize(cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up = normalize(cross(this->Right, this->Front));
	}
};