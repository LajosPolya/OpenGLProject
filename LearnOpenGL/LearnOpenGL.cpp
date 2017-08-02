#include <iostream>
#include <cmath>
#include <map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// My Code
#include "Camera.h"
#include "GameObjectImpl.h"
#include "InstancedArrayGameObjectImpl.h"

// Windows
#include <Windows.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WORLD_LENGTH 5

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();

void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 5.0f, 3.0f));
// Keep track of pressed keys
bool keys[1024];

// Timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Mouse
// Centre of the screen
GLfloat lastX = WIDTH / 2.0f, lastY = HEIGHT / 2.0f;
GLfloat pitch = 0, yaw = -90.0;
// Scroll
GLfloat fov = glm::radians(45.0f);

// Light Source
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Locks Mouse into Screen
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// OpenGL Options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec3 cubePositions[WORLD_LENGTH][WORLD_LENGTH];
	for (GLuint i = 0; i < WORLD_LENGTH; i++) {
		for (GLuint j = 0; j < WORLD_LENGTH; j++) {
			cubePositions[i][j] = glm::vec3((GLfloat)i, (GLfloat)-5, (GLfloat)j);
		}
	}
	
	/* Calcualte Projection Here */
	glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	GameObjectImpl testingGameObject("vertex.vert", "fragment.frag", "container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crate.txt", "Transform/crate.txt", "Material/crate.txt", &camera, projection);
	/* I think the grass Transform.z is 0.5 so it can be rotated properly on the y-axis (so it doesn't rotate along a corner */
	GameObjectImpl grassGameObject("alpha.vert", "alpha.frag", "grass.png", "Mesh/grass.txt", "Transform/grass.txt", &camera, projection);
	/* "vertex.txt", "fragment.txt", "container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crateMaterial.txt", Transform/crate.txt */
	std::vector<GameObjectImpl> transparentGameObjects;
	GameObjectImpl windows("alpha.vert", "blend.frag", "blending_transparent_window.png", "Mesh/window.txt", "Transform/window.txt", &camera, projection);
	transparentGameObjects.push_back(GameObjectImpl("alpha.vert", "blend.frag", "blending_transparent_window.png", "Mesh/window.txt", "Transform/window.txt", &camera, projection));
	transparentGameObjects.push_back(GameObjectImpl("alpha.vert", "blend.frag", "blending_transparent_window.png", "Mesh/window.txt", "Transform/window2.txt", &camera, projection));
	transparentGameObjects.push_back(GameObjectImpl("alpha.vert", "blend.frag", "blending_transparent_window.png", "Mesh/window.txt", "Transform/window3.txt", &camera, projection));
	GameObjectImpl lightBox1("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox1.txt", &camera, projection);
	GameObjectImpl lightBox2("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox2.txt", &camera, projection);
	GameObjectImpl lightBox3("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox3.txt", &camera, projection);
	GameObjectImpl lightBox4("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox4.txt", &camera, projection);

	InstancedArrayGameObjectImpl instancedGameObject("instancedArray.vert", "fragment.frag", "container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crate.txt", "Instance/crate.txt", "Material/crate.txt", &camera, projection);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (GLuint i = 0; i < WORLD_LENGTH; i++) {
			for (GLuint j = 0; j < WORLD_LENGTH; j++) {
				testingGameObject.getTransform()->setPosition(cubePositions[i][j]);
				testingGameObject.Draw();
			}
		}

		glBindVertexArray(0);


		testingGameObject.Draw();

		grassGameObject.Draw();
		grassGameObject.getTransform()->setYRotation(glm::radians(90.0f));
		grassGameObject.Draw();
		grassGameObject.getTransform()->setYRotation(glm::radians(45.0f));
		grassGameObject.Draw();
		grassGameObject.getTransform()->setYRotation(glm::radians(135.0f));
		grassGameObject.Draw();

		instancedGameObject.Draw();


		std::map<float, glm::vec3> sortedWindowPosition;
		for (GLuint i = 0; i < transparentGameObjects.size(); i++) {
			GLfloat distance = glm::length(camera.Position - transparentGameObjects[i].getTransform()->getPosition());
			sortedWindowPosition[distance] = transparentGameObjects[i].getTransform()->getPosition();
		}

		lightBox1.Draw();
		lightBox2.Draw();
		lightBox3.Draw();
		lightBox4.Draw();
		
		GLint i;
		i = 0;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for (std::map<float, glm::vec3>::reverse_iterator it = sortedWindowPosition.rbegin(); it != sortedWindowPosition.rend(); ++it)
		{
			transparentGameObjects[i].getTransform()->setPosition(it->second);
			transparentGameObjects[i].Draw();
			i++;
		}
		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

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

void do_movement() {
	GLfloat cameraSpeed = 5.f * deltaTime;
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

GLboolean firstMouse = true;
void mouse_callback(GLFWwindow * window, double xpos, double ypos) {

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}
