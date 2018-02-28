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
#include "TransparentGameObjectImpl.h"
#include "PerlinNoise.h"
#include "TerrainGenerator.h"
#include "CollisionDetector.h"
#include "ComplexShader.h"
#include "InstancedArrayComplexShader.h"
#include "InstancedComplexShader.h"
#include "SimpleGameObject.h"
#include "SimpleInstancedArrayGameObject.h"
#include "SimpleInstancedGameObject.h"
#include "PositionRelativeCamera.h"
#include "TerrainLoader.h"

#define WORLD_LENGTH 5

// Function prototypes
void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode);
void do_movement();

void mouse_callback(GLFWwindow * window, GLdouble xpos, GLdouble ypos);
void scroll_callback(GLFWwindow * window, GLdouble xoffset, GLdouble yoffset);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Multithreaded Functions
std::vector<PositionRelativeCamera> newReturnQ;
std::vector<SimpleInstancedArrayGameObject> chunks;
std::mutex returnQ_m;
GLuint readyToGrab = 0;
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
int main() {
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
	/* I think the grass Transform.z is 0.5 so it can be rotated properly on the y-axis (so it doesn't rotate along a corner */
	GameObjectImpl grassGameObject("alpha.vert", "alpha.frag", "grass.png", "Mesh/grass.txt", "Transform/grass.txt", camera, projection);
	/* "vertex.txt", "fragment.txt", "container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crateMaterial.txt", Transform/crate.txt */
	GameObjectImpl lightBox1("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox1.txt", camera, projection);
	GameObjectImpl lightBox2("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox2.txt", camera, projection);
	GameObjectImpl lightBox3("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox3.txt", camera, projection);
	GameObjectImpl lightBox4("lamp.vert", "lamp.frag", "Mesh/lightBox.txt", "Transform/lightBox4.txt", camera, projection);

	SimpleInstancedArrayGameObject instancedArrayGameObject = SimpleInstancedArrayGameObject("container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crate.txt", "Instance/crate.txt");
	SimpleInstancedArrayGameObject grassSides("grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", "Instance/crate2.txt");
	// TransparentGameObject is currently broken because it calls setupMesh() which is currently being altered
	TransparentGameObjectImpl instancedWimdowGameObject = TransparentGameObjectImpl("instancedAlpha.vert", "blend.frag", "blending_transparent_window.png,blending_transparent_window.png,blending_transparent_window.png", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Instance/window.txt", camera, projection);
	InstancedTransformImpl newInstancedTransform("Instance/crate3.txt");

	std::vector<glm::vec3> pos2d;
	TerrainGenerator terrainGenerator2d(50, 10, 50, T_2D);
	pos2d = terrainGenerator2d.generate(-50, 0);
	SimpleInstancedArrayGameObject perlin = SimpleInstancedArrayGameObject("container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crate.txt", pos2d);
	chunks.push_back(perlin);
	chunks.push_back(SimpleInstancedArrayGameObject("grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", std::vector<glm::vec3>()));
	chunks.push_back(SimpleInstancedArrayGameObject("grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", std::vector<glm::vec3>()));
	chunks.push_back(SimpleInstancedArrayGameObject("grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", std::vector<glm::vec3>()));
	chunks.push_back(SimpleInstancedArrayGameObject("grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", std::vector<glm::vec3>()));
	chunks.push_back(SimpleInstancedArrayGameObject("grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", std::vector<glm::vec3>()));
	chunks.push_back(SimpleInstancedArrayGameObject("grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", std::vector<glm::vec3>()));
	chunks.push_back(SimpleInstancedArrayGameObject("grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", std::vector<glm::vec3>()));
	chunks.push_back(SimpleInstancedArrayGameObject("grassBlock.jpg,Textures/dirt.jpg,Textures/topGrass.jpg", "Textures/grassBlockSpec.jpg,Textures/dirtSpec.jpg,Textures/topGrassSpec.jpg", "Mesh/toplessCrate.txt,Mesh/bottomSquare.txt,Mesh/floorSquare.txt", "Material/crate.txt", std::vector<glm::vec3>()));

	// Global Lights Container
	LightsContainer globalLightsContainer("Material/crate.txt");
	// ComplexShader
	ComplexShader globalComplexShader(camera, &globalLightsContainer, projection, "vertex.vert", "fragment.frag");
	SimpleGameObject testingGameObject = SimpleGameObject("container2.png", "container2_specular.png", "Mesh/crate.txt", "Material/crate.txt", "Transform/crate.txt");
	globalComplexShader.setSamplers();
	globalComplexShader.sendLightsContainerToShader();
	globalComplexShader.sendProjectionMatrixToShader();
	globalComplexShader.sendToShader(testingGameObject.getMaterial());

	// InstancedComplexShader
	InstancedComplexShader globalInstancedShader(camera, &globalLightsContainer, projection, "instanced.vert", "fragment.frag");
	SimpleInstancedGameObject instancedGameObject = SimpleInstancedGameObject("Textures/coal.jpg", "Textures/coalSpec.jpg", "Mesh/crate.txt", "Material/crate.txt", "Instance/crate1.txt");
	globalInstancedShader.setSamplers();
	globalInstancedShader.sendLightsContainerToShader();
	globalInstancedShader.sendProjectionMatrixToShader();
	globalInstancedShader.sendToShader(instancedGameObject.getMaterial());

	// InstancedArrayComplexShader
	InstancedArrayComplexShader globalInstancedArrayShader(camera, &globalLightsContainer, projection, "instancedArray.vert", "fragment.frag");
	globalInstancedArrayShader.setSamplers();
	globalInstancedArrayShader.sendLightsContainerToShader();
	globalInstancedArrayShader.sendProjectionMatrixToShader();
	globalInstancedArrayShader.sendToShader(perlin.getMaterial());


	// Collision Detection
	CollisionDetector::addCamera(camera);
	CollisionDetector::AddTransform(testingGameObject.getTransform());
	CollisionDetector::AddTransform(lightBox1.getTransform());
	CollisionDetector::AddTransform(lightBox2.getTransform());
	CollisionDetector::AddTransform(lightBox3.getTransform());
	CollisionDetector::AddTransform(lightBox4.getTransform());
	//CollisionDetector::AddTransform(instancedGameObject.getTransform());
	//CollisionDetector::AddTransform(instancedArrayGameObject.getTransform());
	//CollisionDetector::AddTransform(grassSides.getTransform());
	///CollisionDetector::AddTransform(perlin.getTransform());
	//CollisionDetector::AddTransform(perlin3d.getTransform());

	TerrainLoader terrainLoader(camera, returnQ_m, readyToGrab, newReturnQ);
	terrainLoader.start();
	duration = std::clock() - start;
	std::cout << "Time to start: " << duration << std::endl;
	GLuint numFrames = 0;
	GLuint framesPerSec = 0;
	GLuint firstReplace = 0;
	GLfloat totalTime = 0;
	// Game loop
	while (!glfwWindowShouldClose(window)) {
		numFrames++;
		framesPerSec++;

		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = (GLfloat)(currentFrame - lastFrame);
		lastFrame = (GLfloat)currentFrame;
		totalTime += deltaTime;
		if (totalTime > (GLfloat)1.0) {
			std::cout << (GLfloat)framesPerSec / totalTime << std::endl;
			totalTime = (GLfloat)0.0;
			framesPerSec = 0;
		}

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		globalComplexShader.sendCameraToShader();
		globalComplexShader.sendModelToShader(testingGameObject.getTransform()->getModel());
		for (GLuint i = 0; i < WORLD_LENGTH; i++) {
			for (GLuint j = 0; j < WORLD_LENGTH; j++) {
				testingGameObject.getTransform()->setPosition(cubePositions[i][j]);
				globalComplexShader.sendModelToShader(testingGameObject.getTransform()->getModel());
				testingGameObject.Draw();
			}
		}

		// Do Collision before drawing
		CollisionDetector::CheckCollisions();

		// Draw
		globalComplexShader.sendModelToShader(testingGameObject.getTransform()->getModel());
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

		// TODO: Consider switching chunks out every 5 or 10 frames to reduce performance hit
		if (readyToGrab != 0) {
			returnQ_m.lock();
			GLint returnQSize = newReturnQ.size();
			chunks[newReturnQ[returnQSize - 1].getIndex()].setInstances(&newReturnQ[returnQSize - 1].getTransform());
			newReturnQ.pop_back();
			returnQ_m.unlock();
			if (newReturnQ.size() == 0) {
				readyToGrab = 0;
			}
		}

		// TODO: Create LightGameObject so light info is stored withing GameObject
		// TODO: These should be one isntancedArrayGameObject
		// TODO: The Lights should be getting their transform from the Material object or vice versa
		lightBox1.Draw();
		lightBox2.Draw();
		lightBox3.Draw();
		lightBox4.Draw();

		// InstancedComplexShader
		globalInstancedShader.sendCameraToShader();
		if (numFrames == 250) {
			instancedGameObject.setInstances(&newInstancedTransform);
		}
		if (instancedGameObject.hasInstancesChangedAndSetFalse() > 0) {
			globalInstancedShader.sendInstancesToShader(instancedGameObject.getTransform()->getModels());
		}
		instancedGameObject.Draw();

		// Testing InstancedArrayComplexShader and SimpleGameObject
		globalInstancedArrayShader.sendCameraToShader();
		/* Copy Constructor doesn't yet copy LightsContainer */
		for (GLuint i = 0; i < chunks.size(); i++) {
			//perlin.Draw();
			chunks[i].Draw();
		}
		instancedArrayGameObject.Draw();
		grassSides.Draw();

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

	terrainLoader.stop();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow * window, GLint key, GLint scancode, GLint action, GLint mode) {

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
