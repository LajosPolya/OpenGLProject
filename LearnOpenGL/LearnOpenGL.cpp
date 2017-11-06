#include <iostream>
#include <cmath>
#include <map>
#include <ctime>
#include <cstdio>

// Windows
#include <Windows.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// My Code
#include "Camera.h"
#include "GameObjectImpl.h"
#include "InstancedGameObjectImpl.h"
#include "InstancedArrayGameObjectImpl.h"
#include "TransparentGameObjectImpl.h"
#include "PerlinNoise.h"
#include "TerrainGenerator.h"

#define WORLD_LENGTH 5

// Function prototypes
void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode);
void do_movement();

void mouse_callback(GLFWwindow * window, GLdouble xpos, GLdouble ypos);
void scroll_callback(GLFWwindow * window, GLdouble xoffset, GLdouble yoffset);

//Collision
void CheckCollision(GameObjectImpl & gameObject, Camera * camera);
void CheckCollision(InstancedGameObjectImpl & gameObject, Camera * camera);
void CheckCollision(InstancedArrayGameObjectImpl & gameObject, Camera * camera);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera * camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f));
// Previosu Position for Collision Detection
glm::vec3 prevPosition = camera->Position;

// Keep track of pressed keys
bool keys[2048];

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
	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
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
	// For GL_CULL_FACE
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glm::vec3 cubePositions[WORLD_LENGTH][WORLD_LENGTH];
	for (GLuint i = 0; i < WORLD_LENGTH; i++) {
		for (GLuint j = 0; j < WORLD_LENGTH; j++) {
			cubePositions[i][j] = glm::vec3((GLfloat)i, (GLfloat)-5, (GLfloat)j);
		}
	}

	/* Calcualte Projection Here */
	glm::mat4 projection = glm::perspective(camera->Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.01f, 100.0f);

	std::clock_t start;
	GLdouble duration;

	start = std::clock();

	GameObjectImpl testingGameObject("vertex.vert", "fragment.frag", "container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crate.txt", "Transform/crate.txt", "Material/crate.txt", camera, projection);
	/* I think the grass Transform.z is 0.5 so it can be rotated properly on the y-axis (so it doesn't rotate along a corner */
	GameObjectImpl grassGameObject("alpha.vert", "alpha.frag", "grass.png", "Mesh/grass.txt", "Transform/grass.txt", camera, projection);
	/* "vertex.txt", "fragment.txt", "container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crateMaterial.txt", Transform/crate.txt */
	GameObjectImpl lightBox1("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox1.txt", camera, projection);
	GameObjectImpl lightBox2("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox2.txt", camera, projection);
	GameObjectImpl lightBox3("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox3.txt", camera, projection);
	GameObjectImpl lightBox4("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox4.txt", camera, projection);

	InstancedArrayGameObjectImpl instancedArrayGameObject("Shaders/instancedVertToGeo.vert", "fragment.frag", "Shaders/passthrough.geom", "container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crate.txt", "Instance/crate.txt", "Material/crate.txt", camera, projection);
	InstancedArrayGameObjectImpl grassSides("instancedArray.vert", "fragment.frag", "grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", "Instance/crate2.txt", "Material/crate.txt", camera, projection);
	InstancedGameObjectImpl instancedGameObject("instanced.vert", "fragment.frag", "Textures/coal.jpg", "Textures/coalSpec.jpg", "Mesh/crate.txt", "Material/crate.txt", "Instance/crate1.txt", "Material/crate.txt", camera, projection);
	TransparentGameObjectImpl instancedWimdowGameObject("instancedAlpha.vert", "blend.frag", "blending_transparent_window.png,blending_transparent_window.png,blending_transparent_window.png", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Instance/window.txt", camera, projection);

	std::vector<glm::vec3> pos2d;
	TerrainGenerator::generate(50, 50, pos2d);
	std::vector<glm::vec3> pos3d;
	TerrainGenerator::generate(50, 25, 50 , pos3d);

	InstancedArrayGameObjectImpl perlin("instancedArray.vert", "fragment.frag", "container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crate.txt", pos2d, "Material/crate.txt", camera, projection);
	InstancedArrayGameObjectImpl perlin3d("Shaders/instancedVertToGeo.vert", "fragment.frag", "Shaders/passthrough.geom", "grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", pos3d, "Material/crate.txt", camera, projection, GL_TRIANGLES);

	///InstancedArrayGameObjectImpl lineGrass("Shaders/instancedVertToGeo.vert", "Shaders/grass.frag", "Shaders/line.geom", "", "", "Mesh/dynamicGrass.txt", "Material/crate.txt", pos3d, "Material/crate.txt", camera, projection, GL_POINTS);

	duration = std::clock() - start;
	std::cout << "Time to start: " << duration << std::endl;

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = (GLfloat)(currentFrame - lastFrame);
		lastFrame = (GLfloat)currentFrame;
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

		// Do Collision before drawing
		CheckCollision(testingGameObject, camera);
		CheckCollision(lightBox1, camera);
		CheckCollision(lightBox2, camera);
		CheckCollision(lightBox3, camera);
		CheckCollision(lightBox4, camera);
		CheckCollision(instancedGameObject, camera);
		CheckCollision(instancedArrayGameObject, camera);
		CheckCollision(grassSides, camera);
		CheckCollision(perlin, camera);
		CheckCollision(perlin3d, camera);

		// Draw
		testingGameObject.Draw();

		prevPosition = camera->Position;

		glDisable(GL_CULL_FACE);
		grassGameObject.Draw();
		grassGameObject.getTransform()->setYRotation(glm::radians(90.0f));
		grassGameObject.Draw();
		grassGameObject.getTransform()->setYRotation(glm::radians(45.0f));
		grassGameObject.Draw();
		grassGameObject.getTransform()->setYRotation(glm::radians(135.0f));
		grassGameObject.Draw();
		glEnable(GL_CULL_FACE);

		instancedArrayGameObject.Draw();
		instancedGameObject.Draw();
		
		grassSides.Draw();
		perlin.Draw();
		perlin3d.Draw();

		// TODO: These should be one isntancedArrayGameObject
		// TODO: The Lights should be getting their transform from the Material object or vice versa
		lightBox1.Draw();
		lightBox2.Draw();
		lightBox3.Draw();
		lightBox4.Draw();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		instancedWimdowGameObject.Draw();
		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Delete Pointers shared by multiple GameObjects
	GameObjectMemoryManager::deleteSharedPointers();

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode)
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
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S]) {
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A]) {
		camera->ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D]) {
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}
}

GLboolean firstMouse = true;
void mouse_callback(GLFWwindow * window, GLdouble xpos, GLdouble ypos) {

	if (firstMouse) {
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)(xpos - lastX);
	GLfloat yoffset = (GLfloat)(ypos - lastY);
	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow * window, GLdouble xoffset, GLdouble yoffset) {
	camera->ProcessMouseScroll((GLfloat)yoffset);
}

void CheckCollision(GameObjectImpl & gameObject, Camera * camera)
{
	if (gameObject.getTransform()->getPosition().x - 0.8< camera->Position.x && gameObject.getTransform()->getPosition().x + 0.8> camera->Position.x
		&& gameObject.getTransform()->getPosition().y - 0.8< camera->Position.y && gameObject.getTransform()->getPosition().y + 0.8> camera->Position.y
		&& gameObject.getTransform()->getPosition().z - 0.8< camera->Position.z && gameObject.getTransform()->getPosition().z + 0.8> camera->Position.z) {

		if (gameObject.getTransform()->getPosition().x - 0.8< camera->Position.x && gameObject.getTransform()->getPosition().x + 0.8> camera->Position.x
			&& gameObject.getTransform()->getPosition().y - 0.8< prevPosition.y && gameObject.getTransform()->getPosition().y + 0.8> prevPosition.y
			&& gameObject.getTransform()->getPosition().z - 0.8< prevPosition.z && gameObject.getTransform()->getPosition().z + 0.8> prevPosition.z) {

			camera->Position.x = prevPosition.x;
		}

		if (gameObject.getTransform()->getPosition().x - 0.8< prevPosition.x && gameObject.getTransform()->getPosition().x + 0.8> prevPosition.x
			&& gameObject.getTransform()->getPosition().y - 0.8< prevPosition.y && gameObject.getTransform()->getPosition().y + 0.8> prevPosition.y
			&& gameObject.getTransform()->getPosition().z - 0.8< camera->Position.z && gameObject.getTransform()->getPosition().z + 0.8> camera->Position.z) {

			camera->Position.z = prevPosition.z;
		}

		if (gameObject.getTransform()->getPosition().x - 0.8< prevPosition.x && gameObject.getTransform()->getPosition().x + 0.8> prevPosition.x
			&& gameObject.getTransform()->getPosition().y - 0.8< camera->Position.y && gameObject.getTransform()->getPosition().y + 0.8> camera->Position.y
			&& gameObject.getTransform()->getPosition().z - 0.8< prevPosition.z && gameObject.getTransform()->getPosition().z + 0.8> prevPosition.z) {

			camera->Position.y = prevPosition.y;
		}
	}
}

void CheckCollision(InstancedGameObjectImpl & gameObject, Camera * camera)
{
	GLuint i;

	std::vector<glm::vec3> * Positions = gameObject.getTransform()->getPositions();
	GLuint size = Positions->size();
	for (i = 0; i < size; i++) {
		if ((*Positions)[i].x - 0.8< camera->Position.x && (*Positions)[i].x + 0.8> camera->Position.x
			&& (*Positions)[i].y - 0.8< camera->Position.y && (*Positions)[i].y + 0.8> camera->Position.y
			&& (*Positions)[i].z - 0.8< camera->Position.z && (*Positions)[i].z + 0.8> camera->Position.z) {

			if ((*Positions)[i].x - 0.8< camera->Position.x && (*Positions)[i].x + 0.8> camera->Position.x
				&& (*Positions)[i].y - 0.8< prevPosition.y && (*Positions)[i].y + 0.8> prevPosition.y
				&& (*Positions)[i].z - 0.8< prevPosition.z && (*Positions)[i].z + 0.8> prevPosition.z) {

				camera->Position.x = prevPosition.x;
			}

			if ((*Positions)[i].x - 0.8< prevPosition.x && (*Positions)[i].x + 0.8> prevPosition.x
				&& (*Positions)[i].y - 0.8< prevPosition.y && (*Positions)[i].y + 0.8> prevPosition.y
				&& (*Positions)[i].z - 0.8< camera->Position.z && (*Positions)[i].z + 0.8> camera->Position.z) {

				camera->Position.z = prevPosition.z;
			}

			if ((*Positions)[i].x - 0.8< prevPosition.x && (*Positions)[i].x + 0.8> prevPosition.x
				&& (*Positions)[i].y - 0.8< camera->Position.y && (*Positions)[i].y + 0.8> camera->Position.y
				&& (*Positions)[i].z - 0.8< prevPosition.z && (*Positions)[i].z + 0.8> prevPosition.z) {

				camera->Position.y = prevPosition.y;
			}
		}
	}
}

void CheckCollision(InstancedArrayGameObjectImpl & gameObject, Camera * camera)
{
	GLuint i;

	std::vector<glm::vec3> * Positions = gameObject.getTransform()->getPositions();
	GLuint size = Positions->size();
	for (i = 0; i < size; i++) {
		if ((*Positions)[i].x - 0.8< camera->Position.x && (*Positions)[i].x + 0.8> camera->Position.x
			&& (*Positions)[i].y - 0.8< camera->Position.y && (*Positions)[i].y + 0.8> camera->Position.y
			&& (*Positions)[i].z - 0.8< camera->Position.z && (*Positions)[i].z + 0.8> camera->Position.z) {

			if ((*Positions)[i].x - 0.8< camera->Position.x && (*Positions)[i].x + 0.8> camera->Position.x
				&& (*Positions)[i].y - 0.8< prevPosition.y && (*Positions)[i].y + 0.8> prevPosition.y
				&& (*Positions)[i].z - 0.8< prevPosition.z && (*Positions)[i].z + 0.8> prevPosition.z) {

				camera->Position.x = prevPosition.x;
			}

			if ((*Positions)[i].x - 0.8< prevPosition.x && (*Positions)[i].x + 0.8> prevPosition.x
				&& (*Positions)[i].y - 0.8< prevPosition.y && (*Positions)[i].y + 0.8> prevPosition.y
				&& (*Positions)[i].z - 0.8< camera->Position.z && (*Positions)[i].z + 0.8> camera->Position.z) {

				camera->Position.z = prevPosition.z;
			}

			if ((*Positions)[i].x - 0.8< prevPosition.x && (*Positions)[i].x + 0.8> prevPosition.x
				&& (*Positions)[i].y - 0.8< camera->Position.y && (*Positions)[i].y + 0.8> camera->Position.y
				&& (*Positions)[i].z - 0.8< prevPosition.z && (*Positions)[i].z + 0.8> prevPosition.z) {

				camera->Position.y = prevPosition.y;
			}
		}
	}
}
