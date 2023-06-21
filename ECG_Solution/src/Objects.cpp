#include "Objects.h"
#include <cstdlib>
#include <ctime>

Objects::Objects(GLFWwindow* window, Camera* camera, PhysxScene* physxScene)
{
	this->camera = camera;
	this->physxScene = physxScene;
	controllerManager = PxCreateControllerManager(*physxScene->scene);

	//testShadowMap.create();
	debugShader.createDebugShadowShader();

	createTerrain();
	createMummy(window);
	//createPyramid();
	createPalmTree();
	//createVideoWall();
	createPointLightCube();
	//createTreasureChest();
	
	//createTestCube();

	createShadowMap();

	//enemyShader.createPhongShader(glm::mat4(0.0f), 0.1f, 1.0f, 0.2f, 32);
	//enemyShader.setUniform1i("isAnimated", 1);

	// Create Enemies
	for (unsigned int i = 0; i < numEnemies; i++) {
		createEnemy(glm::vec3(10 + i * 6, 3.0f, 2.0f)); //createEnemy(glm::vec3(165 + i * 6, 24.0f, -95.0f));	
	}

	//cactusShader.createPhongShader(glm::mat4(0.0f), 0.1f, 1.0f, 0.1f, 2);
	//cactusShader.setUniform1i("isAnimated", 0);
	//cactusTexture.genTexture("assets/textures/cactus.jpg");

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	for (unsigned int i = 0; i < numCacti; i++) {
		float randomX = static_cast<float>(std::rand() % 1021 - 510);
		float randomZ = static_cast<float>(std::rand() % 1021 - 510);
		createCactus(glm::vec3(randomX, 100.0f, randomZ));
		std::cout << randomX << " Z: " << randomZ << std::endl;
	}
	createCactus(glm::vec3(46.0f, 40.0f, -26));	// one fixed cactus

	//spikeShader.createPhongShader(glm::mat4(0.0f), 0.1f, 0.7f, 0.1f, 2);
	//spikeShader.setUniform1i("isAnimated", 0);

	for (unsigned int i = 0; i < numSpikes; i++) {
		//createSpike();
	}

	//createHduObject(window);

}

void Objects::createTerrain()
{
	const char* heightmap_path = "assets/heightmaps/hm7_pyramid.png"; //hm7_pyramid.png
	terrain.createTerrain("assets/textures/sand.png", heightmap_path);

	physxScene->createTerrain(heightmap_path);
}

void Objects::createPointLightCube()
{
	cubeMat = glm::mat4(1.0f);
	cubeMat = glm::translate(cubeMat, glm::vec3(0.0f, 1.5f, 0.0));
	cubeMat = glm::scale(cubeMat, glm::vec3(0.5f));
	pointLightCube.generateModel("assets/objects/cube2.obj");

	lightCubeShader.createPhongShader("assets/textures/tiles_diffuse.dds", "assets/textures/tiles_specular.dds", "", true, cubeMat, 1, 1, 1, 1);
	lightCubeShader.setUniform1i("isAnimated", 0);
}

void Objects::createMummy(GLFWwindow* window)
{
	mummy.createCharacter(window, camera, controllerManager, physxScene->material, glm::vec3(0.0f, 33.0f, 0.0f));
	// Hill:    -16.0f, 97.0f, -114.0f
	// Pyramid: 17.0f, 32.0f, 76.0f
	// Outside: 103.0f, 30.0f, -63.0f
	physxScene->setCharacter(&mummy);
}

void Objects::createEnemy(glm::vec3 position)
{
	glm::vec3 modelScale = glm::vec3(0.03, 0.03, 0.03);
	Transform modelTransform;
	modelTransform.translate(glm::vec3(0.0, 2.0, 0.0));
	modelTransform.scale(modelScale);

	EnemyStruct enemyStruct;
	enemyStruct.id = enemyCounter;
	enemyStruct.modelMatrix = modelTransform.getMatrix();
	enemyStruct.enemyModel = std::make_shared<Model>("assets/models/eve.dae");
	enemyStruct.enemy = std::make_shared<Enemy>(enemyCounter, enemyStruct.enemyModel, physxScene, controllerManager, modelScale, glm::vec3(position.x, position.y, position.z));

	enemies.push_back(enemyStruct);
	enemyCounter++;
}

void Objects::createCactus(glm::vec3 position)
{
	Transform cactusTransform;
	glm::vec3 scale(glm::vec3(0.005f, 0.005f, 0.005f));
	cactusTransform.translate(position);
	cactusTransform.scale(scale);

	Model* cactusModel = new Model();
	CactusStruct c;
	c.model = cactusModel;
	c.modelMatrix = cactusTransform.getMatrix();

	cactusModel->generateModel("assets/objects/cactus.obj");
	physxScene->createCactus(cactiCounter, cactusModel->modelSize * scale, position);
	cacti[cactiCounter] = c;
	cactiCounter++;
}

void Objects::createPalmTree()
{
	glm::vec3 palmRotate = glm::vec3(glm::radians(-90.0f), glm::radians(0.0f), glm::radians(0.0f));
	glm::vec3 palmScale = glm::vec3(0.01, 0.01, 0.01);
	glm::vec3 palmTranslate = glm::vec3(9.0, 35.8, -10.0);
	Transform palmTransform;
	palmTransform.translate(palmTranslate);
	palmTransform.rotate(palmRotate);
	palmTransform.scale(palmScale);
	palmMatrix = palmTransform.getMatrix();

	glm::vec3 palmTranslate2 = glm::vec3(90.0, 35.8 - 33.0, -100.0);
	Transform palmTransform2;
	palmTransform2.translate(palmTranslate2);
	palmTransform2.rotate(palmRotate);
	palmTransform2.scale(palmScale);
	palmMatrix2 = palmTransform2.getMatrix();
	


	palmTreeShader.createPhongShader("assets/textures/wood_texture.dds", "assets/textures/wood_texture_specular.dds", "",true, palmTransform.getMatrix(), 0.05f, 0.8f, 0.5f, 1.0f);
	palmTreeShader.setUniform1i("isAnimated", 0);

	palmTree.generateModel("assets/objects/palm_tree.obj");

	physxScene->createModel("palmTree", palmTree.indices, palmTree.vertices, palmTree.normals, palmScale, palmTranslate, palmRotate);
}

void Objects::createPyramid()
{
	glm::vec3 pyramidRotate = glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
	glm::vec3 pyramidScale = glm::vec3(40, 45, 40);
	glm::vec3 pyramidTranslate = glm::vec3(-20.0, 31.6, 65.0);    // 31.6
	Transform pyramidTransform;
	pyramidTransform.translate(pyramidTranslate);
	pyramidTransform.rotate(pyramidRotate);
	pyramidTransform.scale(pyramidScale);
	pyramidMatrix = pyramidTransform.getMatrix();
	pyramid.generateModel("assets/objects/pyramid_final1.obj"); //pyramid1	untitled5.obj

	pyramidShader.createPhongShader("assets/textures/sandklein.dds", "assets/textures/sandklein.dds", "", true, pyramidTransform.getMatrix(), 0.05f, 0.8f, 1.0f, 1.0f);
	pyramidShader.setUniform1i("isAnimated", 0);

	physxScene->createModel("pyramid", pyramid.indices, pyramid.vertices, pyramid.normals, pyramidScale, pyramidTranslate, pyramidRotate);
}


void Objects::createSpike()
{
	Model* spikeModel = new Model();
	SpikeStruct spikeStruct;
	spikeStruct.model = spikeModel;
	spikeStruct.render = false;

	spikeModel->generateModel("assets/objects/cone.obj");
	physxScene->createSpike(spikeCounter, spikeModel->modelSize * glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(10.0f, 26.64f, 0.0f));
	spikes[spikeCounter] = spikeStruct;
	spikeCounter++;
}

void Objects::createTumbleweed()
{
	//Model* tumbleWeedModel = new Model();
	//TumbleweedStruct tumbleweedStruct;
	//tumbleweedStruct.model = tumbleWeedModel;

	//tumbleWeedModel->generateModel("assets/objects/tumbleweed.glb");
	//physxScene->createTumbleweed(tumbleweedCounter, tumbleWeedModel->modelSize * glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(10.0f, 26.64f, 0.0f));
	//tumbleweeds[tumbleweedCounter] = tumbleWeedModel;		// TODO: why not working??
	//tumbleweedCounter++;
}

void Objects::createHduObject(GLFWwindow* window)
{
	hduObject.createHdu(window, camera, physxScene->getMummyLiveCount());
	physxScene->setHDU(&hduObject);
}

void Objects::createVideoWall() {
	glm::vec3 videoWallRotate = glm::vec3(glm::radians(90.0f), glm::radians(-90.0f), glm::radians(0.0f));
	glm::vec3 videoWallScale = glm::vec3(80, 80, 80);
	glm::vec3 videoWallTranslate = glm::vec3(-20.0, 54.05, 65.0);
	Transform videoWallTransform;
	videoWallTransform.translate(videoWallTranslate);
	videoWallTransform.rotate(videoWallRotate);
	videoWallTransform.scale(videoWallScale);
	videoWall.generateModel("assets/objects/screenPanel.obj");
	
	videoWallShader.createPhongVideoTexShader("assets/videos/video10_lowQuality.mp4", videoWallTransform.getMatrix(), 0.05f, 0.8f, 1.0f, 1.0f);
	videoWallShader.setUniform1i("isAnimated", 0);

	physxScene->createModel("videowall", videoWall.indices, videoWall.vertices, videoWall.normals, videoWallScale, videoWallTranslate, videoWallRotate);
}

void Objects::createShadowMap()
{
	depthShader.createDepthMapShader();

	// configure depth map FBO
	// -----------------------
	glGenFramebuffers(1, &depthMapFBO);

	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//glm::vec3 lightPosi = glm::vec3(0.01f, 4.0f, 0.0f);
	//glm::vec3 cubePosi = glm::vec3(0.0f, 1.5f, 0.0f);
	//glm::vec3 objectPosi = glm::vec3(90.0f, 3.0f, -100.0f);

	//// Calculate the bounding box that encompasses both objects
	//glm::vec3 minBounds = glm::min(cubePosi, objectPosi);
	//glm::vec3 maxBounds = glm::max(cubePosi, objectPosi);

	//// Expand the bounding box to ensure both objects are fully visible
	//float expandFactor = 1.1f;  // Adjust this value as needed
	//minBounds -= expandFactor;
	//maxBounds += expandFactor;

	//lightPos = glm::vec3(0.01f, 30.0f, 0.0f);
	////lightPos = glm::vec3 ( -2.0f, 4.0f, -1.0f);
	//glm::mat4 orthgonalProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
	//glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 lightModelMat = glm::mat4(1.0f);
	//glm::translate(lightModelMat, glm::vec3(0.0, 0.0, 0.0));
	//lightSpaceMatrix = orthgonalProjection * lightView * lightModelMat;

	// Cameraposition ist am anfang die position von mummy (0.0, 33.0, 0.0) und schaut runter also -y richtung
	// pitch: -89 yaw: -90 um runter zu schauen
	glm::vec3 cameraPosition = glm::vec3(0.0f, 33 + 20.0f, 0.0f);
	// pitch 0 yaw -90 am anfang
	double pitch = -90;
	double yaw = -90;
	// -89, -90: 0, -1, -0.01
	// -90, -90 0, -1, 0
	double radPitch = pitch * M_PI / 180.0;
	double radYaw = yaw * M_PI / 180.0;

	// Calculate necessary angles and positions for the camera transform
	glm::vec3 direction = { cos(radPitch) * cos(radYaw), sin(radPitch), cos(radPitch) * sin(radYaw) };
	glm::vec3 right = glm::normalize(cross(direction, glm::vec3(0.0, 1.0, 0.0)));
	glm::vec3 up = normalize(cross(right, direction));

	glm::vec3 g = direction;
	glm::vec3 e = cameraPosition;
	glm::vec3 t = up;

	glm::vec3 w = -glm::normalize(g);
	glm::vec3 u = glm::normalize(cross(t, w));
	glm::vec3 v = cross(w, u);

	double fov = 60 * M_PI / 180.0;
	double near = 0.5;
	double far = 50;
	double aspect_ratio = 1;
	glm::mat4 perspectiveMatrix = (glm::mat4)glm::perspective(fov, aspect_ratio, near, far);
	lightSpaceMatrix = perspectiveMatrix * glm::inverse(glm::mat4(glm::vec4(u, 0), glm::vec4(v, 0), glm::vec4(w, 0), glm::vec4(e, 1)));


	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	glm::vec3 cameraFront = glm::normalize(front);

	//({0, -1, -1
	std::cout << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << std::endl;
	// 0, 0, -1
}


void Objects::render(GLFWwindow* window, float currentTime, float dt, bool normalMapping)
{
	cubeMat = glm::mat4(1.0f);
	cubeMat = glm::translate(cubeMat, glm::vec3(0.0f, 1.5f, 0.0));
	cubeMat = glm::scale(cubeMat, glm::vec3(0.5f));	
	
	cubeMat2 = glm::mat4(1.0f);
	cubeMat2 = glm::translate(cubeMat2, glm::vec3(0.0f, 33 + 1.3f, -5.0));
	cubeMat2 = glm::scale(cubeMat2, glm::vec3(0.2f));
	
	int screenWidth;
	int screenHeight;
	glfwGetWindowSize(window, &screenWidth, &screenHeight);

	// Character: 
	mummy.pollInput(window, dt);

	glm::mat4 projection = glm::mat4(1.0f);
	viewMatrix = camera->getTransformMatrix();

	eyePos = glm::vec3(camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);

	// simulate physx
	physxScene->simulate(window, camera, (1.0f / 40.0f), spikes, cacti);


	// --------------------------------------------------------------
	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	// Draw model/mesh with shadowMap shader
	renderShadowMap(pointLightCube, cubeMat2);
	//renderShadowMap(pyramid, pyramidMatrix);
	renderShadowMap(palmTree, palmMatrix);
	// Switch back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, screenWidth, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_BACK);


	//---------------------------------------------------------------
	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------------

	//renderModel(&pointLightCube,cubeMat2, &lightCubeShader, false);
	//renderModel(&pyramid, pyramidMatrix, &pyramidShader, false);
	//renderModel(&palmTree, palmMatrix, &palmTreeShader, false);

	debugShader.setUniform1f("near_plane", 0.1);
	debugShader.setUniform1f("far_plane", 1000);
	debugShader.setUniform1i("depthMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	renderDebug();

	// render the terrain
	//terrain.render(viewMatrix,eyePos, lightPos, lightSpaceMatrix, depthMap);

	//renderModel(&treasureChest, treasureChestTransform.getMatrix(), &treasureChestShader, normalMapping);

	//renderTreasureChest(normalMapping);

	//treasureChestShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, viewMatrix);
	//treasureChestShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, treasureChestTransform.getMatrix());
	//treasureChestShader.setUniform3f("eyePos", eyePos.x, eyePos.y, eyePos.z);
	//treasureChestShader.setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	//treasureChestShader.setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);

	//treasureChestShader.setUniform1i("invertNormals", false);
	//treasureChestShader.setUniform1i("normalMapping", false);

	//treasureChest.draw(&treasureChestShader);
	//treasureChest.draw(&treasureChestShader);


	//renderTestCube(normalMapping);

	//testShadowMap.render(viewMatrix, glm::vec3(camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z));

	//render enemies
	//std::vector<unsigned int> enemiesToRemove = physxScene->enemiesToRemove; //get enemies that have been hit 
	//physxScene->enemiesToRemove.clear();

	//enemyShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	//enemyShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);

	//for (unsigned int i = 0; i < enemies.size(); i++)
	//{
	//	if (std::find(enemiesToRemove.begin(), enemiesToRemove.end(), i + 1) != enemiesToRemove.end()) //if enemy was hit by spike
	//	{
	//		enemies[i].enemy->isDead = true;
	//	}

	//	enemies[i].enemy->move(enemyShader, enemies[i].modelMatrix, currentTime, mummy.getPosition(), 50, dt); //move physx enemy controller & render object

	//	if (enemies[i].enemy->shouldBeDeleted)
	//	{
	//		enemies.erase(enemies.begin() + i); //remove enemy if dying animation is done 
	//	}
	//}

	// ALL ENEMIES DEAD!! YOU WON
	//if (enemies.size() == 0 && !physxScene->allEnemiesDead) {
	//	//hduObject.showBigScreen("winEndscreen");
	//	physxScene->allEnemiesDead = true;
	//	std::cout << "All enemies are dead! Get the treasure chest!" << std::endl;
	//}

	//// render cactus
	//for (const auto& pair : cacti) {
	//	cactusTexture.bind(0);
	//	cactusTexture.bind(1);
	//	cactusShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, pair.second.modelMatrix);
	//	cactusShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	//	cactusShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
	//	pair.second.model->draw(&cactusShader);
	//}


	////render light cube
	//lightCubeShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	//lightCubeShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
	//pointLightCube.draw(&lightCubeShader);

	////render spikes
	//for (const auto& pair : spikes) {
	//	if (pair.second.render) {
	//		Transform t;
	//		t.translate(pair.second.translate);
	//		t.rotate(pair.second.rotate);
	//		t.scale(pair.second.scale);
	//		spikeShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, t.getMatrix());
	//		spikeShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	//		spikeShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
	//		pair.second.model->draw(&spikeShader);
	//	}
	//}
	// render videowall
	//videoWallShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	//videoWallShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
	//elapsedTime += dt;
	//videoWallShader.setCurrentFrame(elapsedTime);
	//videoWall.draw(&videoWallShader);

	//renderTestCube(normalMapping, testCubeShader);
	//// Draw HDU last
	//hduObject.drawHDU(window);

}

void Objects::renderShadowMap(Model& model, glm::mat4 modelMatrix)
{
	depthShader.setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);
	depthShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, modelMatrix);
	model.draw(nullptr);
}

void Objects::renderModel(Model* model, glm::mat4 modelMatrix, Shader* shader, bool normalMapping)
{

	shader->setShadowMap(depthMap);
	shader->setUniform3f("lightDir", 0, 1, 0);

	shader->setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, viewMatrix);
	shader->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, modelMatrix);
	shader->setUniform3f("eyePos", eyePos.x, eyePos.y, eyePos.z);
	shader->setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	shader->setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	shader->setUniform1i("shadowMap", 3);

	shader->setUniform1i("invertNormals", false);
	shader->setUniform1i("normalMapping", normalMapping);
	shader->setUniform1i("withShadow", true);
	model->draw(nullptr);
}

void Objects::renderTreasureChest(bool normalMapping) {

	treasureChestShader.setShadowMap(depthMap);
	treasureChestShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, viewMatrix);
	treasureChestShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, treasureChestTransform.getMatrix());
	treasureChestShader.setUniform3f("eyePos", eyePos.x, eyePos.y, eyePos.z);
	treasureChestShader.setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	treasureChestShader.setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);

	treasureChestShader.setUniform1i("invertNormals", false);
	treasureChestShader.setUniform1i("normalMapping", normalMapping);

	treasureChestShader.setUniform1i("withShadow", true);

	treasureChest.draw(&treasureChestShader);
}

void Objects::createTreasureChest() {
	glm::vec3 chestRotate = glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
	glm::vec3 chestScale = glm::vec3(0.5, 0.5, 0.5);
	glm::vec3 chestTranslate = glm::vec3(6.0, 3.3 + 25.64, 2.0); //glm::vec3(108.0, 29, -43.0);

	treasureChestTransform.translate(chestTranslate);
	treasureChestTransform.rotate(chestRotate);
	treasureChestTransform.scale(chestScale);

	treasureChestShader.createPhongShader("assets/textures/chest/chest_baseColor.jpg",
		"assets/textures/chest/chest_specular_metallic.jpg", "assets/textures/chest/chest_normal.jpg", false, treasureChestTransform.getMatrix(), 0,0,0,0);

	treasureChest.generateModel("assets/objects/treasureChest.obj");

	//physxScene->createModel("treasureChest", treasureChest.indices, treasureChest.vertices, treasureChest.normals, chestScale, chestTranslate, chestRotate);
}


void Objects::deleteObjects()
{
	terrain.deleteTerrain();
	physxScene->deleteScene();
}

void Objects::renderTestCube(bool normalMapping)
{
	//render test cube
	Transform t;
	t.translate(glm::vec3(1.0f, 27.0, 0.0));

	testCubeShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, t.getMatrix());
	testCubeShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	testCubeShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);


	glm::vec3 diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f) * 1.5f; // lightColor * diffuseStrength
	glm::vec3 ambientColor = diffuseColor * 0.3f;//diffuseColor * ambientStrength;

	testCubeShader.setUniform3f("light.position", 1.2f, 28.0f, 2.0f);
	testCubeShader.setUniform3f("light.ambientIntensity", ambientColor.x, ambientColor.y, ambientColor.z);
	testCubeShader.setUniform3f("light.diffuseIntensity", diffuseColor.x, diffuseColor.y, diffuseColor.z);
	testCubeShader.setUniform3f("light.specularIntensity", 1.0f, 1.0f, 1.0f);
	testCubeShader.setUniform1f("light.constant", 1);
	testCubeShader.setUniform1f("light.linear", 0.10f);
	testCubeShader.setUniform1f("light.quadratic", 0.065f);

	testCubeShader.setUniform1i("disableAttenuation", true);
	testCubeShader.setUniform1i("invertNormals", false);
	testCubeShader.setUniform1i("normalMapping", normalMapping);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

//TODO berechnung in drawables verschieben
void Objects::createTestCube()
{
	Transform t;
	t.translate(glm::vec3(40.0f, 30.0, -13.0));
	testCubeShader.createNormalShader("assets/textures/brickwall.jpg", "assets/textures/brickwall.jpg", "assets/textures/brickwall_normal.jpg", t.getMatrix());

	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		 // bottom face
		 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 // top face
		 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	constexpr int numVertices = sizeof(vertices) / (8 * sizeof(float));

	float tangents[numVertices * 3];
	float bitangents[sizeof(tangents)];

	for (int vertex = 0; vertex < numVertices; vertex += 3)
	{
		glm::vec3 v0 = glm::vec3(vertices[vertex * 8 + 0], vertices[vertex * 8 + 1], vertices[vertex * 8 + 2]);
		glm::vec3 v1 = glm::vec3(vertices[(vertex + 1) * 8 + 0], vertices[(vertex + 1) * 8 + 1], vertices[(vertex + 1) * 8 + 2]);
		glm::vec3 v2 = glm::vec3(vertices[(vertex + 2) * 8 + 0], vertices[(vertex + 2) * 8 + 1], vertices[(vertex + 2) * 8 + 2]);

		const uint32_t st_offset = 6;
		glm::vec2 st0 = glm::vec2(vertices[vertex * 8 + st_offset + 0], vertices[vertex * 8 + st_offset + 1]);
		glm::vec2 st1 = glm::vec2(vertices[(vertex + 1) * 8 + st_offset + 0], vertices[(vertex + 1) * 8 + st_offset + 1]);
		glm::vec2 st2 = glm::vec2(vertices[(vertex + 2) * 8 + st_offset + 0], vertices[(vertex + 2) * 8 + st_offset + 1]);

		glm::vec3 edge1_xyz = v1 - v0;
		glm::vec3 edge2_xyz = v2 - v0;

		glm::vec2 edge1_st = st1 - st0;
		glm::vec2 edge2_st = st2 - st0;


		float determinant = 1 / ((edge1_st.x * edge2_st.y) - (edge2_st.x * edge1_st.y));
		glm::mat2 adjugate(1.0f);

		adjugate[0][0] = determinant * edge2_st.y;  //column0
		adjugate[0][1] = determinant * -edge2_st.x; //column0
		adjugate[1][0] = determinant * -edge1_st.y; //column1
		adjugate[1][1] = determinant * edge1_st.x;  //column1

		glm::mat2& inverseMatrix = adjugate;

		glm::vec3 tangent;
		tangent.x = inverseMatrix[0][0] * edge1_xyz.x + inverseMatrix[1][0] * edge2_xyz.x;
		tangent.y = inverseMatrix[0][0] * edge1_xyz.y + inverseMatrix[1][0] * edge2_xyz.y;
		tangent.z = inverseMatrix[0][0] * edge1_xyz.z + inverseMatrix[1][0] * edge2_xyz.z;
		tangent = glm::normalize(tangent);

		glm::vec3 bitangent;
		bitangent.x = inverseMatrix[0][1] * edge1_xyz.x + inverseMatrix[1][1] * edge2_xyz.x;
		bitangent.y = inverseMatrix[0][1] * edge1_xyz.y + inverseMatrix[1][1] * edge2_xyz.y;
		bitangent.z = inverseMatrix[0][1] * edge1_xyz.z + inverseMatrix[1][1] * edge2_xyz.z;
		bitangent = glm::normalize(bitangent);

		for (int adjacent = 0; adjacent < 3; ++adjacent)
		{
			tangents[vertex * 3 + adjacent * 3 + 0] = tangent.x;
			tangents[vertex * 3 + adjacent * 3 + 1] = tangent.y;
			tangents[vertex * 3 + adjacent * 3 + 2] = tangent.z;

			bitangents[vertex * 3 + adjacent * 3 + 0] = bitangent.x;
			bitangents[vertex * 3 + adjacent * 3 + 1] = bitangent.y;
			bitangents[vertex * 3 + adjacent * 3 + 2] = bitangent.z;
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &tangentsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tangentsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tangents), tangents, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(3);

	glGenBuffers(1, &bitangentsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bitangents), bitangents, GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);

}

void Objects::renderDebug()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}