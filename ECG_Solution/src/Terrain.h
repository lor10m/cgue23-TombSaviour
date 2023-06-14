#pragma once

#include "Utils/Utils.h"
#include <vector>
#include "Utils/stb_image.h"
#include "Shader.h"
#include "PxPhysicsAPI.h"
#include "Texture.h"

class Terrain {

private:
	Texture heightmapTexture;
	Texture surfaceTexture;

	std::vector<float> vertices;
	unsigned int terrainVAO, terrainVBO;
	unsigned int NUM_PATCH_PTS = 4;
	unsigned rez = 20;
	int width, height, nrChannels;
	Shader* shader;

public:
	Terrain(const char* texturePath, const char* heightmapPath);
	Terrain();
	void render(Shader* shader, GLuint depthMap);
	void deleteTerrain();
	int verticesCount;
	physx::PxVec3 physicsVertices[20*20*4];
};