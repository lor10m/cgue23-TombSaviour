#include "Objects.h"
#include <cstdlib>
#include <ctime>

Objects::Objects(GLFWwindow* window, Camera* camera, PhysxScene* physxScene, bool withVideoWall)
{
	this->camera = camera;
	this->physxScene = physxScene;
	controllerManager = PxCreateControllerManager(*physxScene->scene);

	this->withVideoWall = withVideoWall;
	debugShader.createDebugShadowShader();
	
	createTerrain();
	createMummy(window);
	createPyramid();
	
	std::cout << withVideoWall << std::endl;
	if (withVideoWall) {
		createVideoWall();
	}

	//createPointLightCube();
	createTreasureChest();

	createShadowMap();

	//---------------
	// Create Enemies
	//---------------
	enemyShader.createPhongShader(glm::mat4(0.0f), ambientFactor, diffuseFactor, specularFactor, alpha);
	enemyShader.setUniform1i("isAnimated", 1);
	enemyShader.setUniform1i("withShadow", 0);
	enemyShader.setUniform1i("videoWall", 0);
	for (unsigned int i = 0; i < numEnemies; i++) {
		createEnemy(glm::vec3(165 + i * 6, 34.0f, -95.0f)); //createEnemy(glm::vec3(165 + i * 6, 24.0f, -95.0f));	
	}

	//------------
	// Create Cacti
	//-------------
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	cactusShader.createPhongShader("assets/textures/Cactus.jpg", "assets/textures/Cactus.jpg", "", false, glm::mat4(1.0f), ambientFactor + 3.0f, diffuseFactor, specularFactor, alpha);
	cactusShader.setUniform1i("isAnimated", 0);
	cactusShader.setUniform1i("withShadow", 0);
	cactusShader.setUniform1i("videoWall", 0);

	createCactus(glm::vec3(46.0f, 40.0f, -26));	// one fixed cactus
	createCactus(glm::vec3(50.0f, 40.0f, -30));	// one fixed cactus
	createCactus(glm::vec3(185.0, 55.0, 36.5));
	createCactus(glm::vec3(450.0, 0.0, -142.0));
	createCactus(glm::vec3(423.0, 0.0, -380.0));
	createCactus(glm::vec3(-108.0, 28.0, 304.0));
	createCactus(glm::vec3(122.0, 51.0, 248.0));
	createCactus(glm::vec3(116.472, 37.1818, 10.7676));
	createCactus(glm::vec3(107.325, 32, -17.2375));

	//------------
	// Create Palm trees
	//-------------
	palmTreeShader.createPhongShader("assets/textures/Cactus.jpg", "assets/textures/Cactus.jpg", "", false, glm::mat4(1.0f), ambientFactor + 2.0f, 0.8f, 0.5f, 1.0f);
	palmTreeShader.setUniform1i("isAnimated", 0);
	palmTreeShader.setUniform1i("withShadow", 0);
	palmTreeShader.setUniform1i("videoWall", 0);

	createPalmTree(glm::vec3(10.5, 93.0, -126.5));
	createPalmTree(glm::vec3(204.0, 28.0, -53.0));
	createPalmTree(glm::vec3(104.0, 28.0, -53.0));
	createPalmTree(glm::vec3(192.0, 69.0, 168.5));
	createPalmTree(glm::vec3(104.53, 48, 73.1072));
	//createPalmTree(glm::vec3(-295.0, 48.0, -92.0));
	//createPalmTree(glm::vec3(303.0, 29.0, 132.0));
	//createPalmTree(glm::vec3(70.0, 40.8, -100.0));
	//createPalmTree(glm::vec3(90.0, 39.8, -80.0));
	createPalmTree(glm::vec3(45.0, 31.0, -18.0));
	//-20.0, 31.6, 65.0
	//------------
	// Create Spikes
	//------------
	spikeShader.createPhongShader("assets/textures/Cactus.jpg", "assets/textures/Cactus.jpg", "", false, glm::mat4(1.0f), ambientFactor, diffuseFactor, specularFactor, alpha);
	spikeShader.setUniform1i("isAnimated", 0);
	spikeShader.setUniform1i("withShadow", 0);
	spikeShader.setUniform1i("videoWall", 0);
	for (unsigned int i = 0; i < numSpikes; i++) {
		createSpike();
	}

	//------------
	// Create Tumbleweed
	//------------
	//tumbleweedShader.createPhongShader("assets/textures/Cactus.jpg", "assets/textures/Cactus.jpg", "", false, glm::mat4(1.0f), ambientFactor, diffuseFactor, specularFactor, alpha);
	//tumbleweedShader.setUniform1i("isAnimated", 0);
	//tumbleweedShader.setUniform1i("withShadow", 0);
	//tumbleweedShader.setUniform1i("videoWall", 0);
	//for (unsigned int i = 0; i < numTumbleweeds; i++) {
	//	createTumbleweed();
	//}

	createHduObject(window);
	 
}

void Objects::createTerrain()
{
	const char* heightmap_path = "assets/heightmaps/hm7_pyramid.png"; //hm7_pyramid.png
	terrain.createTerrain("assets/textures/sand.png", heightmap_path, ambientFactor, 0.3, 0.1, 30);

	physxScene->createTerrain(heightmap_path);
}

void Objects::createPointLightCube()
{
	cubeMat = glm::mat4(1.0f);
	cubeMat = glm::translate(cubeMat, glm::vec3(0.0f, 1.5f, 0.0));
	cubeMat = glm::scale(cubeMat, glm::vec3(0.5f));
	pointLightCube.generateModel("assets/objects/cube2.obj");

	lightCubeShader.createPhongShader("assets/textures/ground.png", "assets/textures/ground.png", "", false, cubeMat, 0.0f, 0.5, 0.3, 64);
	lightCubeShader.setUniform1i("isAnimated", 0);
	lightCubeShader.setUniform1i("videoWall", 0);
	lightCubeShader.setUniform1i("withShadow", 1);
}

void Objects::createMummy(GLFWwindow* window)
{
	mummy.createCharacter(window, camera, controllerManager, physxScene->material, glm::vec3(103.0f, 100, -64.0));
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
	enemyStruct.enemy->difficulty = difficulty;
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

void Objects::createPalmTree(glm::vec3 position)
{
	Model* palmTree = new Model();
	PalmStruct palmStruct;
	palmStruct.model = palmTree;

	glm::vec3 palmRotate = glm::vec3(glm::radians(-90.0f), glm::radians(0.0f), glm::radians(0.0f));
	glm::vec3 palmScale = glm::vec3(0.01, 0.01, 0.01);
	glm::vec3 palmTranslate = glm::vec3(position.x, position.y, position.z);
	Transform palmTransform;
	palmTransform.translate(palmTranslate);
	palmTransform.rotate(palmRotate);
	palmTransform.scale(palmScale);
	palmStruct.modelMatrix = palmTransform.getMatrix();

	palmTree->generateModel("assets/objects/palm_tree.obj");
	palms[palmCounter] = palmStruct;
	palmCounter++;

	//physxScene->createModel("palmTree", palmTree->indices, palmTree->vertices, palmTree->normals, palmScale, palmTranslate, palmRotate);
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

	pyramidShader.createPhongShader("assets/textures/ground.png", "assets/textures/ground.png", "", false, pyramidTransform.getMatrix(), 1.0f, 0.3, 0.1, 30);
	pyramidShader.setUniform1i("isAnimated", 0);
	pyramidShader.setUniform1i("withShadow", 0);
	pyramidShader.setUniform1i("videoWall", 0);

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
	//glm::vec3 r = glm::vec3(glm::radians(-90.0f), glm::radians(0.0f), glm::radians(0.0f));
	//glm::vec3 s = glm::vec3(0.01, 0.01, 0.01);
	//glm::vec3 t = glm::vec3(10,10,10);
	//Transform tran;
	//tran.translate(t);
	//tran.rotate(r);
	//tran.scale(s);

	//tumbleWeedModel->generateModel("assets/objects/tumbleweed.glb");
	//physxScene->createModel("tumbleweed", tumbleWeedModel.>indices, tumbleWeedModel.vertices, pyramid.normals, pyramidScale, pyramidTranslate, pyramidRotate);
	//tumbleweeds[tumbleweedCounter] = tumbleweedStruct;
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
	videoWallShader.setUniform1i("normalMapping", false);
	videoWallShader.setUniform1i("withShadow", false);

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

	glm::vec3 lightPosition = glm::vec3(-31.0f, 33 + 15.3f, 64.0);
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
	glm::vec3 e = lightPosition;
	glm::vec3 t = up;

	glm::vec3 w = -glm::normalize(g);
	glm::vec3 u = glm::normalize(cross(t, w));
	glm::vec3 v = cross(w, u);

	double fov = 20 * M_PI / 180.0;
	double near = 0.5;
	double far = 100;
	double aspect_ratio = camera->aspect_ratio;
	std::cout << aspect_ratio << std::endl;
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
	cubeMat2 = glm::translate(cubeMat2, glm::vec3(-30.0f, 31.0f, 64.0)); 
	cubeMat2 = glm::scale(cubeMat2, glm::vec3(10.0f, 0.1f, 10.0f));

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
	//renderShadowMap(pointLightCube, cubeMat2);
	renderShadowMap(treasureChest, treasureChestTransform.getMatrix());
	//renderShadowMap(pyramid, pyramidMatrix);
	//renderShadowMap(palmTree, palmMatrix);
	
	// Switch back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, screenWidth, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_BACK);


	//---------------------------------------------------------------
	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------------

	//debugShader.setUniform1f("near_plane", 0.1);
	//debugShader.setUniform1f("far_plane", 1000);
	//debugShader.setUniform1i("depthMap", 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	//renderDebug();


	//render pyramid
	renderModel(&pyramid, pyramidMatrix, &pyramidShader, false, false);

	// render palm
	for (const auto& pair : palms) {
		renderModel(pair.second.model, pair.second.modelMatrix, &palmTreeShader, false, false);
	}

	// render the terrain
	terrain.render(viewMatrix, eyePos, lightPos, lightSpaceMatrix, depthMap);

	// render treasure
	if (turnRadius == 360.0f) {
		turnRadius = 0.0f;
	}
	turnRadius += 0.0002f;
	treasureChestTransform.rotate(glm::vec3(glm::radians(0.0f), glm::radians(turnRadius), glm::radians(0.0f)));
	renderModel(&treasureChest, treasureChestTransform.getMatrix(), &treasureChestShader, normalMapping, false);

	//renderModel(&pointLightCube, cubeMat2, &lightCubeShader, false, true);

	// render cactus
	for (const auto& pair : cacti) {
		renderModel(pair.second.model, pair.second.modelMatrix, &cactusShader, false, false);
	}

	//render spikes
	for (const auto& pair : spikes) {
		if (pair.second.render) {
			Transform t;
			t.translate(pair.second.translate);
			t.rotate(pair.second.rotate);
			t.scale(pair.second.scale);
			renderModel(pair.second.model, t.getMatrix(), &spikeShader, false, false);
		}
	}

	for (const auto& pair : tumbleweeds) {
		Transform t;
		t.translate(pair.second.translate);
		t.rotate(pair.second.rotate);
		t.scale(pair.second.scale);
		renderModel(pair.second.model, t.getMatrix(), &tumbleweedShader, false, false);
	}
	//render enemies
	std::vector<unsigned int> enemiesToRemove = physxScene->enemiesToRemove; //get enemies that have been hit 
	physxScene->enemiesToRemove.clear();

	enemyShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	enemyShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
	enemyShader.setUniform1i("normalMapping", false);
	enemyShader.setUniform1i("withShadow", false);

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		if (std::find(enemiesToRemove.begin(), enemiesToRemove.end(), i + 1) != enemiesToRemove.end()) //if enemy was hit by spike
		{
			enemies[i].enemy->isDead = true;
		}

		enemies[i].enemy->move(enemyShader, enemies[i].modelMatrix, currentTime, mummy.getPosition(), 50, dt); //move physx enemy controller & render object

		if (enemies[i].enemy->shouldBeDeleted)
		{
			enemies.erase(enemies.begin() + i); //remove enemy if dying animation is done 
		}
	}

	// ALL ENEMIES DEAD => touch treasure chest and you win!
	if (enemies.size() == 0) {
		if (!mummy.allEnemiesDead) {
			mummy.allEnemiesDead = true;
			std::cout << "All enemies are dead! Get the treasure chest!" << std::endl;
		}
		if (mummy.treasureChestTouch && showEndScreenOnce) {
			showEndScreenOnce = false; // set this so that we don't call this method multiple times at the end
			hduObject.showBigScreen("winEndscreen");
		}
	}

	//render videowall
	if (withVideoWall && (mummy.getFootPosition().x >= -60 && mummy.getFootPosition().x <= 90 && mummy.getFootPosition().z <= 105 && mummy.getFootPosition().z >= 25)) {
		videoWallShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
		videoWallShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
		videoWallShader.setUniform1i("videoWall", true);
		elapsedTime += dt;
		videoWallShader.setCurrentFrame(elapsedTime);
		videoWall.draw(&videoWallShader);
	}

	if (instructionScreenActive) {
		if (hduObject.pollInput(window, dt)) {
			instructionScreenActive = false;
			//std::cout << "game initialized!" << std::endl;
		}
	}

	// Draw HDU last
	hduObject.drawHDU(window);

}

void Objects::renderShadowMap(Model& model, glm::mat4 modelMatrix)
{
	depthShader.setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);
	depthShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, modelMatrix);
	model.draw(nullptr);
}

void Objects::renderModel(Model* model, glm::mat4 modelMatrix, Shader* shader, bool normalMapping, bool withShadow)
{

	shader->setUniform3f("lightDir", 0, 1, 0);
	
	shader->setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, viewMatrix);
	shader->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, modelMatrix);
	shader->setUniform3f("eyePos", eyePos.x, eyePos.y, eyePos.z);
	shader->setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	shader->setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);
	shader->setUniform1i("invertNormals", false);
	shader->setUniform1i("normalMapping", normalMapping);
	shader->setUniform1i("withShadow", withShadow);

	if (withShadow) {
		shader->setShadowMap(depthMap);
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shader->setUniform1i("shadowMap", 3);
	}
	model->draw(nullptr);
}

void Objects::createTreasureChest() {
	glm::vec3 chestRotate = glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
	glm::vec3 chestScale = glm::vec3(3.0, 3.0, 3.0);
	glm::vec3 chestTranslate = glm::vec3(-31, 35, 64);

	treasureChestTransform.translate(chestTranslate);
	treasureChestTransform.rotate(chestRotate);
	treasureChestTransform.scale(chestScale);

	treasureChestShader.createPhongShader("assets/textures/chest/chest_baseColor.jpg",
		"assets/textures/chest/chest_specular_metallic.jpg", "assets/textures/chest/chest_normal.jpg", false, treasureChestTransform.getMatrix(),ambientFactor + 3.0f, 1.8, 0.2, alpha);

	treasureChest.generateModel("assets/objects/treasureChest.obj");
	treasureChestShader.setUniform1i("isAnimated", 0);
	treasureChestShader.setUniform1i("withShadow", 0);
	treasureChestShader.setUniform1i("videoWall", 0);

	physxScene->createModel("treasureChest", treasureChest.indices, treasureChest.vertices, treasureChest.normals, chestScale, chestTranslate, chestRotate);
}


void Objects::deleteObjects()
{
	terrain.deleteTerrain();
	physxScene->deleteScene();
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