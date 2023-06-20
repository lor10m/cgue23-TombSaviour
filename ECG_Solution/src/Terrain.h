#pragma once

#include "Utils/Utils.h"
#include <vector>
#include "Utils/stb_image.h"
#include "Shader.h"
#include "PxPhysicsAPI.h"
#include "Texture.h"
#include "Drawables/Model.h"

class Terrain {

private:
	float test = -50.0f;
	Texture heightmapTexture;
	Texture specularTexture;
	Texture diffuseTexture;

	Texture diffCubeTex;
	Texture specCubeTex;

	unsigned int SHADOW_WIDTH = 2048;  //1024
	unsigned int SHADOW_HEIGHT = 2048;
	unsigned int depthMapFBO;
	unsigned int depthMap;

	std::vector<float> vertices;
	std::vector<float> indices;
	int indexCount;
	unsigned int terrainVAO, terrainVBO;
	unsigned int NUM_PATCH_PTS = 4;
	unsigned rez = 20;
	int width, height, nrChannels;
	Shader shader;
	Shader depthShader;
	Shader cubeShader;
	Model cube;
	Model pyramid;

public:
	void createTerrain(const char* texturePath, const char* heightmapPath);
	Terrain();
	void render(glm::mat4 viewMatrix, glm::vec3 eyePos);
	void deleteTerrain();
	int verticesCount;
	physx::PxVec3 physicsVertices[20*20*4];
};