#include "PlayerController.h"



PlayerController::PlayerController(GLfloat & deltaTime, Camera & camera, GLfloat lastX, GLfloat lastY) : camera(camera), deltaTime(deltaTime) {
	this->lastX = lastX;
	this->lastY = lastY;
}


PlayerController::~PlayerController() {}

// Is called whenever a key is pressed/released via GLFW
void PlayerController::key_callback(GLFWwindow * window, GLint key, GLint scancode, GLint action, GLint mode) {

	if (action == GLFW_PRESS) {
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void PlayerController::do_movement() {
	if (keys[GLFW_KEY_W]) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void PlayerController::mouse_callback(GLFWwindow * window, GLdouble xpos, GLdouble ypos) {

	if (firstMouse) {
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)(xpos - lastX);
	GLfloat yoffset = (GLfloat)(ypos - lastY);
	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void PlayerController::scroll_callback(GLFWwindow * window, GLdouble xoffset, GLdouble yoffset) {
	camera.ProcessMouseScroll((GLfloat)yoffset);
}
