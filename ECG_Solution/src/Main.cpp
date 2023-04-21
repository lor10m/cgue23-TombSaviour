#include "Utils/Utils.h"
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

using namespace physx;

double camera_fov = 60 * 3.141592 / 180.0;
double camera_near = 0.1;
double camera_far = 1000;

bool polygonMode = false;


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	if (!glfwInit()) {
		EXIT_WITH_ERROR("Failed to init GLFW");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	INIReader reader("assets/settings.ini");
	GLFWwindow* window = glfwCreateWindow(reader.GetInteger("window", "width", 800), ("window", "height", 800), "LearnOpenGL: Terrain GPU", nullptr, nullptr);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);

	glewExperimental = true;
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

    Camera camera(window, camera_fov, (double)800 / (double)800, camera_near, camera_far);
    
    // build and compile our shader program
    // ------------------------------------
	
	// OBJECTS
	PhysxScene physxScene;
	Objects objects(&camera, &physxScene);

    glfwSetWindowUserPointer(window, &camera);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);


	// -----------
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		float lastFrame = 0.0f;
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera.pollMousePosition(window);

		// render
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render objects
		objects.render(window, deltaTime);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();

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