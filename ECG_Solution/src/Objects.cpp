#include "Objects.h"

Objects::Objects(GLFWwindow* window, Camera* camera, PhysxScene* physxScene)
{
	this->camera = camera;
	this->physxScene = physxScene;
	controllerManager = PxCreateControllerManager(*physxScene->scene);

	createTerrain();
	createMummy(window);
	createPyramid();
	createPalmTree();
	createVideoWall();
	createPointLightCube();
	createTestCube();

	enemyShader.createPhongShader(glm::mat4(0.0f), 0.1f, 1.0f, 0.2f, 32);
	enemyShader.setUniform1i("isAnimated", 1);

	// Create Enemies
	for (unsigned int i = 0; i < numEnemies; i++) {
		createEnemy(glm::vec3(10 + i * 6, 3.0f, 2.0f));
	}

	cactusShader.createPhongShader(glm::mat4(0.0f), 0.1f, 1.0f, 0.1f, 2);
	cactusShader.setUniform1i("isAnimated", 0);
	cactusTexture.genTexture("assets/textures/cactus.jpg");

	for (unsigned int i = 0; i < numCacti; i++) {
		createCactus(glm::vec3(10.0f + 4 * i, 25.64f, i * 2.0f));
	}

	spikeShader.createPhongShader(glm::mat4(0.0f), 0.1f, 0.7f, 0.1f, 2);
	spikeShader.setUniform1i("isAnimated", 0);

	for (unsigned int i = 0; i < numSpikes; i++) {
		createSpike();
	}

	createHduObject(window);

}

void Objects::createTerrain()
{

	terrainShader.createTerrainShader();

	const char* heightmap_path = "assets/heightmaps/hm4_dark.png";
	terrain = Terrain(&terrainShader, "assets/textures/sand.png", heightmap_path);

	physxScene->createTerrain(heightmap_path);

}

void Objects::createMummy(GLFWwindow* window)
{
	mummy.createCharacter(window, camera, controllerManager, physxScene->material, glm::vec3(0.0f, 30.0f, 0.0f));
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
	enemyStruct.enemy = std::make_shared<Enemy>(enemyCounter, enemyStruct.enemyModel, physxScene, controllerManager, modelScale, glm::vec3(position.x, 28.64f, position.z));

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
	glm::vec3 palmTranslate = glm::vec3(0.0, 25.64, 30.0);
	Transform palmTransform;
	palmTransform.translate(palmTranslate);
	palmTransform.rotate(palmRotate);
	palmTransform.scale(palmScale);

	palmTreeShader.createPhongShader("assets/textures/wood_texture.dds", "assets/textures/wood_texture_specular.dds", palmTransform.getMatrix(), 0.05f, 0.8f, 0.5f, 1.0f);
	palmTreeShader.setUniform1i("isAnimated", 0);

	palmTree.generateModel("assets/objects/palm_tree.obj");

	physxScene->createModel("palmTree", palmTree.indices, palmTree.vertices, palmTree.normals, palmScale, palmTranslate, palmRotate);
}

void Objects::createPyramid()
{
	glm::vec3 pyramidRotate = glm::vec3(glm::radians(0.0f), glm::radians(90.0f), glm::radians(0.0f));
	glm::vec3 pyramidScale = glm::vec3(5, 5, 5);
	glm::vec3 pyramidTranslate = glm::vec3(16.0, 25.64, 15.0);
	Transform pyramidTransform;
	pyramidTransform.translate(pyramidTranslate);
	pyramidTransform.rotate(pyramidRotate);
	pyramidTransform.scale(pyramidScale);
	pyramid.generateModel("assets/objects/pyramid_final1.obj"); //pyramid1	untitled5.obj

	pyramidShader.createPhongShader("assets/textures/sandklein.dds", "assets/textures/sandklein.dds", pyramidTransform.getMatrix(), 0.05f, 0.8f, 1.0f, 1.0f);
	pyramidShader.setUniform1i("isAnimated", 0);

	physxScene->createModel("pyramid", pyramid.indices, pyramid.vertices, pyramid.normals, pyramidScale, pyramidTranslate, pyramidRotate);
}

void Objects::createPointLightCube()
{
	Transform t;
	t.translate(glm::vec3(0.0f, 30.0, -7.0));
	t.scale(glm::vec3(0.1, 0.1, 0.1));
	pointLightCube.generateModel("assets/objects/cube.obj");

	lightCubeShader.createPhongShader("assets/textures/weiss.dds", "assets/textures/weiss.dds", t.getMatrix(), 1, 1, 1, 1);
	lightCubeShader.setUniform1i("isAnimated", 0);
}


//TODO delete when collision
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

void Objects::createHduObject(GLFWwindow* window)
{
	hduObject.createHdu(window, camera, physxScene->getMummyLiveCount());
	physxScene->setHDU(&hduObject);
}

void Objects::createVideoWall()
{
	glm::vec3 videoWallRotate = glm::vec3(glm::radians(0.0f), glm::radians(90.0f), glm::radians(0.0f));
	glm::vec3 videoWallScale = glm::vec3(10, 10, 10);
	glm::vec3 videoWallTranslate = glm::vec3(45.0, 25, -5.0);
	Transform videoWallTransform;
	videoWallTransform.translate(videoWallTranslate);
	videoWallTransform.rotate(videoWallRotate);
	videoWallTransform.scale(videoWallScale);
	videoWall.generateModel("assets/objects/screen.obj");

	videoWallShader.createPhongVideoTexShader("assets/videos/videoShort.mp4", videoWallTransform.getMatrix(), 0.05f, 0.8f, 1.0f, 1.0f);
	videoWallShader.setUniform1i("isAnimated", 0);

	physxScene->createModel("videowall", videoWall.indices, videoWall.vertices, videoWall.normals, videoWallScale, videoWallTranslate, videoWallRotate);
}


void Objects::render(GLFWwindow* window, float currentTime, float dt, bool normalMapping)
{
	// ALL ENEMIES DEAD!!
	if (enemies.size() == 0) {
		hduObject.drawHDU(window);
		return;
	}

	// YOU ARE DEAD!!
	if (physxScene->getMummyLiveCount() == 0) {
		hduObject.drawHDU(window);
		return;
	}

	//TODO maybe load textures in Model.cpp instead of shader and use 1 shader for (almost) all objects
	// Character: 
	mummy.pollInput(window, dt);

	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 viewMatrix = camera->getTransformMatrix();

	// simulate physx
	physxScene->simulate(window, camera, (1.0f / 40.0f), spikes, cacti);

	//render enemies
	std::vector<unsigned int> enemiesToRemove = physxScene->enemiesToRemove; //get enemies that have been hit 
	physxScene->enemiesToRemove.clear();

	enemyShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	enemyShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);

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

	if (enemies.size() == 0) {
		hduObject.showBigScreen("winEndscreen");
		std::cout << "\nYou've won for now but we will get you soon! >:(" << std::endl;
	}

	// render the terrain
	terrainShader.setUniformMatrix4fv("projection", 1, GL_FALSE, projection);
	terrainShader.setUniformMatrix4fv("view", 1, GL_FALSE, viewMatrix);
	terrainShader.setUniformMatrix4fv("model", 1, GL_FALSE, glm::mat4(1.0f));
	terrain.render();

	// render cactus
	for (const auto& pair : cacti) {
		cactusTexture.bind(0);
		cactusTexture.bind(1);
		cactusShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, pair.second.modelMatrix);
		cactusShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
		cactusShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
		pair.second.model->draw(&cactusShader);
	}

	// render pyramid
	pyramidShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	pyramidShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
	pyramid.draw(&pyramidShader);

	// render palm tree
	palmTreeShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	palmTreeShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
	palmTree.draw(&palmTreeShader);

	// render videowall
	videoWallShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	videoWallShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
	videoWall.draw(&videoWallShader);

	//render light cube
	//lightCubeShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
	//lightCubeShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
	//pointLightCube.draw(&lightCubeShader);

	//render spikes
	for (const auto& pair : spikes) {
		if (pair.second.render) {
			Transform t;
			t.translate(pair.second.translate);
			t.rotate(pair.second.rotate);
			t.scale(pair.second.scale);
			spikeShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, t.getMatrix());
			spikeShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
			spikeShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
			pair.second.model->draw(&spikeShader);
		}
	}


	renderTestCube(normalMapping);
	
	// Draw HDU last
	hduObject.drawHDU(window);

}

void Objects::deleteObjects()
{
	terrain.deleteTerrain();
	physxScene->deleteScene();
}

//TODO berechnung in drawables verschieben bzw. removen und für das object in der pyramide verwenden
void Objects::createTestCube()
{
	Transform t;
	t.translate(glm::vec3(5.0f, 27.0, 25.0));
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

void Objects::renderTestCube(bool normalMapping)
{
	//render test cube
	Transform t;
	t.translate(glm::vec3(5.0f, 27.0, 25.0));

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