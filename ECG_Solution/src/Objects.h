#pragma once

#include "Utils/Utils.h"
#include "Terrain.h"
#include "PhysxScene.h"
#include "Character.h"
#include "Utils/Transform.h"
#include "Enemy.h"
#include "Drawables/Cube.h"
#include "Hdu.h"
#include "Utils/GlobalVariables.h"
#include "DepthMap.h"


struct EnemyStruct {
	unsigned int id;
	std::shared_ptr<Model> enemyModel = nullptr;
	std::shared_ptr<Enemy> enemy = nullptr;
	glm::mat4 modelMatrix;
};


class Objects {

private:
	PhysxScene* physxScene;
	PxControllerManager* controllerManager;
	Camera* camera;

	Hdu hduObject;

	//DepthMap terrainDepthMap;
	unsigned int SHADOW_WIDTH = 2048;  //1024
	unsigned int SHADOW_HEIGHT = 2048;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	glm::vec3 lightPos;
	glm::mat4 lightSpaceMatrix;

	Shader depthShader;
	Terrain terrain;
	Model pyramid;
	Model videoWall;
	Model pointLightCube;
	Model treasureChest;

	unsigned int vao;
	unsigned int vbo;
	unsigned int tangentsVBO;
	unsigned int bitangentsVBO;

	Shader debugShader;
	Shader testCubeShader;
	Shader shadowShader;

	Shader terrainDepthMapShader;
	Shader terrainShader;
	Shader palmTreeShader;
	Shader enemyShader;
	Shader pyramidShader;
	Shader cactusShader;
	Shader spikeShader;
	Shader tumbleweedShader;
	Shader videoWallShader;
	Shader lightCubeShader;
	Shader treasureChestShader;
	Transform treasureChestTransform;

	Character mummy;
	Texture cactusTexture;
	Shader hduShader;

	unsigned int spikeCounter = 0;
	unsigned int tumbleweedCounter = 0;
	unsigned int palmCounter = 0;
	unsigned int cactiCounter = 0;
	unsigned int enemyCounter = 1; // has to start at 1 because 0 can't be used for enemy's pxChar userData

	std::vector<unsigned int> deadEnemyIndices;
	std::vector<EnemyStruct> enemies;

	std::map<unsigned int, SpikeStruct> spikes;
	std::map<unsigned int, TumbleweedStruct> tumbleweeds;
	std::map<unsigned int, CactusStruct> cacti;
	std::map<unsigned int, PalmStruct> palms;
	

	unsigned int numEnemies = 1;
	unsigned int numCacti = 4;
	unsigned int numSpikes = 5 * numCacti;
	unsigned int numTumbleweeds = 0;
	unsigned int numPalms = 1;
	void createSpike();
	void createTumbleweed();

	double physxDeltaTime = 0.0;
	float elapsedTime = 0.0f;
	
	float turnRadius = 0.0f;

	void renderShadowMap(Model& model, glm::mat4 modelMatrix);
	void renderModel(Model* model, glm::mat4 modelMatrix, Shader* shader, bool normalMapping, bool withShadow);

	glm::mat4 viewMatrix;
	glm::vec3 eyePos;

	glm::mat4 pyramidMatrix;
	glm::mat4 palmMatrix;
	glm::mat4 palmMatrix2;
	

	glm::mat4 cubeMat;
	glm::mat4 cubeMat2;

	double StartTimeInstScr = glfwGetTime();
	bool instructionScreenActive = true;
	bool showEndScreenOnce = true;
	
public:

	int alpha = 64;
	float diffuseFactor = 0.8;
	float specularFactor = 0.4;
	float ambientFactor = 0.8;


	Objects(GLFWwindow* window, Camera* camera, PhysxScene* physxScene);
	void render(GLFWwindow* window, float currentTime, float dt, bool normalMapping);
	void createMummy(GLFWwindow* window);
	void createTerrain();
	void createEnemy(glm::vec3 position);
	void createPyramid();
	void createPalmTree(glm::vec3 position);
	void createPointLightCube();
	void createHduObject(GLFWwindow* window);
	void createVideoWall();
	void createTreasureChest();
	
	void deleteObjects();
	void createCactus(glm::vec3 position);

	void createShadowMap();

	unsigned int quadVAO = 0;;
	unsigned int quadVBO;
	void renderDebug();
};