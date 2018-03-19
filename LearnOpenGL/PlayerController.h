#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>

#include "Camera.h"


/// https://stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback
class PlayerController
{
public:
	PlayerController(GLfloat & deltaTime, Camera & camera, GLfloat lastX, GLfloat lastY);
	~PlayerController();

	// Function prototypes
	void key_callback(GLFWwindow * window, GLint key, GLint scancode, GLint action, GLint mode);
	void do_movement();

	void mouse_callback(GLFWwindow * window, GLdouble xpos, GLdouble ypos);
	void scroll_callback(GLFWwindow * window, GLdouble xoffset, GLdouble yoffset);

	GLboolean getPutDownLight();
	void setPutDownLightFalse();

private:
	Camera & camera;

	// Keep track of pressed keys
	bool keys[2048];

	// Mouse
	// Centre of the screen
	GLfloat lastX;
	GLfloat lastY;;

	GLboolean firstMouse = true;

	// Timing
	GLfloat & deltaTime;

	GLboolean putDownLight = false;
};

