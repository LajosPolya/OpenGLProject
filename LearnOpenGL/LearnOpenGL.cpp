#include <iostream>
#include <cmath>
#include <map>
#include <ctime>
#include <cstdio>
#include <queue>
#include <thread>
#include <chrono>
#include <mutex>

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
#include "CollisionDetector.h"

#define WORLD_LENGTH 5

// Function prototypes
void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode);
void do_movement();

void mouse_callback(GLFWwindow * window, GLdouble xpos, GLdouble ypos);
void scroll_callback(GLFWwindow * window, GLdouble xoffset, GLdouble yoffset);

// Multithreaded Functions
byte killAll = 0;
std::vector<InstancedArrayGameObjectImpl> chunks;
std::mutex pc_m;
void Producer();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera * camera = new Camera(glm::vec3(0.0f, 0.0f, -10.0f));
// Previosu Position for Collision Detection
glm::vec3 prevPosition = camera->Position;

/* Calcualte Projection Here */
glm::mat4 projection = glm::perspective(camera->Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.01f, 100.0f);

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
	///glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

		
	std::vector<glm::vec3> pos3d;
	TerrainGenerator terrainGenerator3d(50, 25, 50, T_3D);
	pos3d = terrainGenerator3d.generate(0, 0, 0);
	InstancedArrayGameObjectImpl perlin3d("Shaders/instancedVertToGeo.vert", "fragment.frag", "Shaders/passthrough.geom", "grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", pos3d, "Material/crate.txt", camera, projection, GL_TRIANGLES);

	std::vector<glm::vec3> pos3d2;
	pos3d2 = terrainGenerator3d.generate(0, 0, 50);
	InstancedArrayGameObjectImpl perlin3d2("Shaders/instancedVertToGeo.vert", "fragment.frag", "Shaders/passthrough.geom", "grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", pos3d2, "Material/crate.txt", camera, projection, GL_TRIANGLES);

	std::vector<glm::vec3> pos3d3;
	pos3d3 = terrainGenerator3d.generate(-50, 0, 0);
	InstancedArrayGameObjectImpl perlin3d3("Shaders/instancedVertToGeo.vert", "fragment.frag", "Shaders/passthrough.geom", "grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", pos3d3, "Material/crate.txt", camera, projection, GL_TRIANGLES);

	///InstancedArrayGameObjectImpl lineGrass("Shaders/instancedVertToGeo.vert", "Shaders/grass.frag", "Shaders/line.geom", "", "", "Mesh/dynamicGrass.txt", "Material/crate.txt", pos3d, "Material/crate.txt", camera, projection, GL_POINTS);

	duration = std::clock() - start;
	std::cout << "Time to start: " << duration << std::endl;

	CollisionDetector::addCamera(camera);
	CollisionDetector::AddTransform(testingGameObject.getTransform());
	CollisionDetector::AddTransform(lightBox1.getTransform());
	CollisionDetector::AddTransform(lightBox2.getTransform());
	CollisionDetector::AddTransform(lightBox3.getTransform());
	CollisionDetector::AddTransform(lightBox4.getTransform());
	CollisionDetector::AddTransform(instancedGameObject.getTransform());
	CollisionDetector::AddTransform(instancedArrayGameObject.getTransform());
	CollisionDetector::AddTransform(grassSides.getTransform());
	///CollisionDetector::AddTransform(perlin.getTransform());
	CollisionDetector::AddTransform(perlin3d.getTransform());

	std::vector<glm::vec3> pos2d;
	TerrainGenerator terrainGenerator2d(50, 10, 50, T_2D);
	pos2d = terrainGenerator2d.generate(-50, 0);
	InstancedArrayGameObjectImpl perlin("instancedArray.vert", "fragment.frag", "container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crate.txt", pos2d, "Material/crate.txt", camera, projection);
	chunks.push_back(perlin);

	std::thread t1(Producer);
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
		CollisionDetector::CheckCollisions();

		// Draw
		testingGameObject.Draw();

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
		///perlin.Draw();
		perlin3d.Draw();
		perlin3d2.Draw();
		perlin3d3.Draw();

		// TODO: These should be one isntancedArrayGameObject
		// TODO: The Lights should be getting their transform from the Material object or vice versa
		lightBox1.Draw();
		lightBox2.Draw();
		lightBox3.Draw();
		lightBox4.Draw();

		/* Copy Constructor doesn't yet copy LightsContainer */
		for (GLuint i = 0; i < chunks.size(); i++) {
			//perlin.Draw();
			chunks[i].Draw();
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		instancedWimdowGameObject.Draw();
		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);

		// Set prevPosition
		CollisionDetector::SetPrevPosiion(camera->Position);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Delete Pointers shared by multiple GameObjects
	GameObjectMemoryManager::deleteSharedPointers();

	killAll = 1;
	t1.detach();
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

void Producer() {
	///while (killAll != 1) {
		pc_m.lock();
		///std::this_thread::sleep_for(std::chrono::seconds(5));
		//std::cout << "Slept for 5 Seconds" << std::endl;
		pc_m.unlock();
	///}
}
