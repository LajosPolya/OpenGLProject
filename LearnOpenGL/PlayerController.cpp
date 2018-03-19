#include "PlayerController.h"



PlayerController::PlayerController(GLfloat & deltaTime, Camera & camera, GLfloat lastX, GLfloat lastY) : camera(camera), deltaTime(deltaTime) {
	this->lastX = lastX;
	this->lastY = lastY;
}


PlayerController::~PlayerController() {}

// Is called whenever a key is pressed/released via GLFW
void PlayerController::key_callback(GLFWwindow * window, GLint key, GLint scancode, GLint action, GLint mode) {

	if (action == GLFW_PRESS) {
		this->keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		this->keys[key] = false;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void PlayerController::do_movement() {
	if (this->keys[GLFW_KEY_W]) {
		this->camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (this->keys[GLFW_KEY_S]) {
		this->camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (this->keys[GLFW_KEY_A]) {
		this->camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (this->keys[GLFW_KEY_D]) {
		this->camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (this->keys[GLFW_KEY_B]) {
		this->keys[GLFW_KEY_B] = false;
		this->putDownLight = true;
	}
}

void PlayerController::mouse_callback(GLFWwindow * window, GLdouble xpos, GLdouble ypos) {

	if (this->firstMouse) {
		this->lastX = (GLfloat)xpos;
		this->lastY = (GLfloat)ypos;
		this->firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)(xpos - this->lastX);
	GLfloat yoffset = (GLfloat)(ypos - this->lastY);
	this->lastX = (GLfloat)xpos;
	this->lastY = (GLfloat)ypos;

	this->camera.ProcessMouseMovement(xoffset, yoffset);
}

void PlayerController::scroll_callback(GLFWwindow * window, GLdouble xoffset, GLdouble yoffset) {
	this->camera.ProcessMouseScroll((GLfloat)yoffset);
}

GLboolean PlayerController::getPutDownLight() {
	return this->putDownLight;
}

void PlayerController::setPutDownLightFalse() {
	this->putDownLight = false;
}
