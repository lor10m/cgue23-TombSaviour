#pragma once

#include "Utils/Utils.h"
#include "Terrain.h"
#include "PhysxScene.h"
#include "Character.h"
#include "Utils/Transform.h"
#include "Enemy.h"
#include "ShaderManager.h"
#include "Drawables/Cube.h"
#include "Hdu.h"
#include "Utils/GlobalVariables.h"
#include "DepthMap.h"
#include "ShadowMap.h"


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

	ShadowMap testShadowMap;

	//DepthMap terrainDepthMap;
	unsigned int SHADOW_WIDTH = 2048;  //1024
	unsigned int SHADOW_HEIGHT = 2048;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	Shader depthShader;
	Terrain terrain;
	Model palmTree;
	Model pyramid;
	Model videoWall;
	Model pointLightCube;

	unsigned int vao;
	unsigned int vbo;
	unsigned int tangentsVBO;
	unsigned int bitangentsVBO;

	Shader testCubeShader;
	Shader shadowShader;

	Shader terrainDepthMapShader;
	Shader terrainShader;
	Shader palmTreeShader;
	Shader pyramidShader;
	Shader cactusShader;
	Shader spikeShader;
	Shader videoWallShader;
	Shader lightCubeShader;

	Character mummy;
	Texture cactusTexture;
	Shader hduShader;

	Shader enemyShader;
	//std::shared_ptr<Model> enemyModel = nullptr;
	//std::shared_ptr<Enemy> enemy = nullptr;
	//Enemy enemy;

	unsigned int spikeCounter = 0;
	unsigned int cactiCounter = 0;
	unsigned int enemyCounter = 1; // has to start at 1 because 0 can't be used for enemy's pxChar userData

	std::vector<unsigned int> deadEnemyIndices;

	std::vector<EnemyStruct> enemies;

	//std::map<unsigned int, EnemyStruct> enemies;
	std::map<unsigned int, SpikeStruct> spikes;
	std::map<unsigned int, CactusStruct> cacti;
	
	unsigned int numEnemies = 1;
	unsigned int numCacti = 3;
	unsigned int numSpikes = 10 * numCacti;
	void createSpike();

	double physxDeltaTime = 0.0;

	void renderTestCube(bool normalMapping, Shader& shader);
	
public:

	Objects(GLFWwindow* window, Camera* camera, PhysxScene* physxScene);
	void render(GLFWwindow* window, float currentTime, float dt, bool normalMapping);
	void createMummy(GLFWwindow* window);
	void createTerrain();
	void createEnemy(glm::vec3 position);
	void createPyramid();
	void createPalmTree();
	void createPointLightCube();
	void createTestCube();
	void createHduObject(GLFWwindow* window);
	void createVideoWall();
	void deleteObjects();
	void createCactus(glm::vec3 position);
};