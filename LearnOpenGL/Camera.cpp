#include "Camera.h"

Camera::Camera(vec3 position, vec3 up, GLfloat yaw, GLfloat pitch) : Front(vec3(1.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
	this->Position = vec3(posX, posY, posZ);
	this->WorldUp = vec3(upX, upY, upZ);
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

mat4 Camera::GetViewMatrix() {
	return lookAt(this->Position, this->Position + this->Front, this->Up);
}

const mat4 Camera::GetViewMatrix() const {
	return lookAt(this->Position, this->Position + this->Front, this->Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
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

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch) {
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

void Camera::ProcessMouseScroll(GLfloat yoffset) {
	if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
		this->Zoom -= yoffset;
	if (this->Zoom <= 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom >= 45.0f)
		this->Zoom = 45.0f;
}

const vec3 & Camera::getPosition() const {
	return this->Position;
}

vec3 & Camera::getPosition() {
	return this->Position;
}

const vec3 & Camera::getFront() const {
	return this->Front;
}

const GLfloat & Camera::getZoom() const {
	return this->Zoom;
}

void Camera::updateCameraVectors() {
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