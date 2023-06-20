#include "Utils/Utils.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Utils/stb_image.h"
#include <iostream>
#include <vector>
#include "PxPhysicsAPI.h"
#include "Camera.h"
#include "Terrain.h"
#include "PhysxScene.h"
#include "Character.h"
#include "Enemy.h"
#include "Drawables/Model.h"
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "ShaderManager.h"
#include "Utils/Transform.h"
#include "Utils/Callbacks.h"
#include "Objects.h"
#include "Hdu.h"
#include <thread>

using namespace physx;

double camera_fov = 60 * 3.141592 / 180.0;
double camera_near = 0.1;
double camera_far = 1000;

bool polygonMode = false;


int main()
{
	// get values from ini file
	INIReader reader("assets/settings.ini");
	int window_width = reader.GetInteger("window", "width", 800);
	int window_height = reader.GetInteger("window", "height", 800);
	int refresh_rate = reader.GetInteger("window", "refresh_rate", 60);
	std::string window_title = reader.Get("window", "title", "Tomb Saviour");
	double camera_fov = reader.GetReal("camera", "fov", 60) * M_PI / 180.0;
	double camera_near = reader.GetReal("camera", "near", 0.1);
	double camera_far = reader.GetReal("camera", "far", 1000);

	// glfw: initialize and configure
	// ------------------------------
	if (!glfwInit()) {
		EXIT_WITH_ERROR("Failed to init GLFW");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// set refresh rate:
	glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);

#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Tomb Saviour", nullptr, nullptr);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);

	// TODO illumination multiplier
	int brightnessIdx = reader.GetReal("global", "brightnessIdx", 10);
	setIllumination(brightnessIdx);
	std::cout << "Illumination: " << getIlluminationMultiplier() << "\n";
	// https://github.com/lor10m/cgue23-TombSaviour/blob/CameraEtc/ECG_Solution/src/GlobalVariables.cpp
	int lifeNumber = reader.GetReal("global", "maxlifes", 3);

	//glewExperimental = true;
	if (GLEW_OK != glewInit()) {
		EXIT_WITH_ERROR("Failed to init GLEW");
	}

	if (!initFramework()) {
		EXIT_WITH_ERROR("Failed to init framework");
	}

	glEnable(GL_DEPTH_TEST);

	if (polygonMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}

    Camera camera(window, camera_fov, camera_near, camera_far, false);

    // build and compile our shader program
    // ------------------------------------
	
	// OBJECTS
	PhysxScene physxScene(window, lifeNumber);
	Objects objects(window, &camera, &physxScene);
	
    glfwSetWindowUserPointer(window, &camera);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	double deltaTime = 0.0;
	double physxDeltaTime = 0.0;
	double oneUnit, velocity = 0.0;
	float minDeltaTime = 1.0f / 60.0f; // min delta time or minfps = 60
	
	float lastFrame = 0.0f;

	double initialTime = glfwGetTime();
	int frameCount = 0;
	double finalTime = 0;

	// -----------
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		//calculate frame rate
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame; //frame rate
		lastFrame = currentFrame;

		// render
		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);		// white/grey
		//glClearColor(0.7f, 0.7f, 1.0f, 1.0f);		// light blue
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.pollMousePosition(window, isShooterCam());

		//std::cout << "currentFrame: " << currentFrame << std::endl;
		objects.render(window, currentFrame, deltaTime, normalMappingActivated());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();

		// calculate actual FPS
		frameCount++;
		finalTime = glfwGetTime();
		if (finalTime - initialTime >= 1) 
		{
			//std::cout << "FPS: " << frameCount / (finalTime - initialTime) << std::endl;
			frameCount = 0;
			initialTime = finalTime;
		}
	}

	objects.deleteObjects();

	glfwTerminate();
	return 0;
}

glm::mat4 aiMatrix4x4ToGlm(aiMatrix4x4 from)
{
	glm::mat4 to;

	to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
	to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
	to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
	to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

	return to;
}