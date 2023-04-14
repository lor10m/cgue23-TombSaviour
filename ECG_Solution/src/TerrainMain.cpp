#include "Utils.h"
#include "stb_image.h"
#include "TerrainShader.h"
#include "TerrainCamera.h"
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Terrain.h"
#include "PhysxScene.h"
#include "PhysxCamera.h"

#include "Drawables/Model.h"
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Drawables/Animation.h"
#include "Drawables/Bone.h"
#include "Drawables/Animator.h"
#include "Callbacks.h"


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

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	if (polygonMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}

	Camera camera(window, camera_fov, (double)800 / (double)800, camera_near, camera_far);

	// build and compile our shader program
	// ------------------------------------

	// TERRAIN
	TerrainShader tessHeightMapShader("assets/shaders/terrainVertex.vs", "assets/shaders/terrainFragment.fs", nullptr,
		"assets/shaders/terrain.tcs", "assets/shaders/terrain.tes");

	Terrain terrain(tessHeightMapShader, "assets/textures/sand2.jpg", "assets/heightmaps/hm3-darkk.png");

	PhysxScene physxScene;
	physxScene.createTerrain("assets/heightmaps/hm3-darkk.png");
	//physxScene.createPlayer();

	// OBJECT
	PointLight pointLight1({ 0, 0, 0 }, { 1, 1, 1 }, { 1.0f, 0.4f, 0.1f });
	DirectionalLight directionalLight1({ 0, -1, -1 }, { 0.8f, 0.8f, 0.8f });

	ShaderManager shaderManager;
	shaderManager.addPointLight(pointLight1);
	shaderManager.addDirectionalLight(directionalLight1);


	// Animated Model:
	glm::vec3 modelRotate = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 modelScale = glm::vec3(0.01, 0.01, 0.01);
	Transform modelTransform;
	modelTransform.translate(glm::vec3(0.0, 0.0, 0.0));
	modelTransform.rotate(modelRotate);
	modelTransform.scale(modelScale);

	Model model_animated("assets/models/eve.dae");

	Shader* animatedModelShader = shaderManager.createPhongShader(0.1f, 0.7f, 0.1f, 2);
	animatedModelShader->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, modelTransform.getMatrix());
	animatedModelShader->setUniform1i("isAnimated", 1);

	physxScene.createModel(model_animated.indices, model_animated.vertices, model_animated.normals, glm::vec3(10, 10, 10), modelRotate);


	// Palm tree:
	Shader* palmTreeShader = shaderManager.createPhongShader("assets/textures/wood_texture.dds", "assets/textures/wood_texture_specular.dds", 0.1f, 0.7f, 0.1f, 2);

	glm::vec3 palmRotate = glm::vec3(glm::radians(-90.0), 0.0, 0.0);
	glm::vec3 palmScale = glm::vec3(0.01, 0.01, 0.01);
	Transform palmTransform;
	palmTransform.translate(glm::vec3(0.0, 0.0, 0.0));
	palmTransform.rotate(palmRotate);
	palmTransform.scale(palmScale);

	Model model_palmTree("assets/objects/palm_tree.obj");

	palmTreeShader->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, palmTransform.getMatrix());
	palmTreeShader->setUniform1i("isAnimated", 0);

	physxScene.createModel(model_palmTree.indices, model_palmTree.vertices, model_palmTree.normals, glm::vec3(10, 10, 10), palmRotate);

	// Pyramid: 
	Shader* pyramidShader = shaderManager.createPhongShader("assets/textures/wood_texture.dds", "assets/textures/wood_texture_specular.dds", 0.1f, 0.7f, 0.1f, 2);

	glm::vec3 pyramidRotate = glm::vec3(glm::radians(-90.0), 0.0, 0.0);
	glm::vec3 pyramidScale = glm::vec3(1, 1, 1);
	Transform pyramidTransform;
	pyramidTransform.translate(glm::vec3(0.0, 0.0, 0.0));
	pyramidTransform.rotate(pyramidRotate);
	pyramidTransform.scale(pyramidScale);
	Model model_pyramid("assets/objects/pyramid1.obj");

	glm::mat4 animTransform = glm::mat4(1.0f);

	pyramidShader->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, animTransform);

	physxScene.createModel(model_pyramid.indices, model_pyramid.vertices, model_pyramid.normals, pyramidScale, pyramidRotate);


	glfwSetWindowUserPointer(window, &camera);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	float animationTimeTicks = 0.0f;
	float ticksPerSecond = model_animated.getTicksPerSecond();
	float animationDuration = model_animated.getAnimationDuration();
	//glm::mat4 animTransform = glm::mat4(1.0f);

	std::vector<glm::mat4> transformationMatricesCopy(model_animated.finalBoneMatrices.size(), glm::mat4(1.0f));
	std::vector<glm::mat4> transformationMatrices;

	glm::mat4 sa = glm::mat4(1.0f);
	//glm::mat4 boneTransform = bones[boneIDs[0]] * weights[0];
	//boneTransform += bones[boneIDs[1]] * weights[1];
	//boneTransform += bones[boneIDs[2]] * weights[2];
	//boneTransform += bones[boneIDs[3]] * weights[3];

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		float lastFrame = 0.0f;
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		physxScene.simulate(window, 1.0f / 60.0f);
		camera.pollInput(window);
		camera.pollMousePosition(window);

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// render the terrain
		terrain.render();
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = camera.getTransformMatrix();

		tessHeightMapShader.setMat4("projection", projection);
		tessHeightMapShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		tessHeightMapShader.setMat4("model", model);


		// render model
		shaderManager.updateCameraValues(camera);

		pyramidShader->activate();
		model_pyramid.draw(pyramidShader);

		palmTreeShader->activate();
		model_palmTree.draw(palmTreeShader);

		//render animated Model
		animatedModelShader->activate();

		if (animationTimeTicks > fmod(deltaTime * ticksPerSecond, animationDuration)) {
			animationTimeTicks = 0.0f;

			glm::mat4 x = model_animated.tr;
			//sa = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 3.0, 0.0));
			//animTransform *= x;

		}

		animationTimeTicks = fmod(deltaTime * ticksPerSecond, animationDuration);

		model_animated.getBoneTransforms(deltaTime, animTransform, transformationMatrices);
		for (unsigned int i = 0; i < transformationMatrices.size(); i++) {
			glm::mat4 mat = transformationMatrices[i];
			animatedModelShader->setUniformMatrix4fv("bones[" + std::to_string(i) + "]", 1, GL_FALSE, mat);
		}

		model_animated.draw(animatedModelShader);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	terrain.deleteTerrain();

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