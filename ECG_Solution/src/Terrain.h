#pragma once

#include "Utils.h"
#include <vector>
#include "stb_image.h"
#include "TerrainShader.h"
#include "PxPhysicsAPI.h"
#include "Texture.h"

class Terrain {

private:
	//void genHeatMap();
	//void genTerrainTexture(const char* texturePath);
	//unsigned int heightMapTexture;
	//unsigned int surfaceTexture;
	Texture heightmapTexture;
	Texture surfaceTexture;

	std::vector<float> vertices;
	unsigned int terrainVAO, terrainVBO;
	const unsigned int NUM_PATCH_PTS = 4;
	unsigned rez = 20;
	int width, height, nrChannels;
	TerrainShader shader;

public:
	Terrain(TerrainShader tessHeightMapShader, const char* texturePath, const char* heightmapPath);
	void render();
	void deleteTerrain();
	int verticesCount;
	physx::PxVec3 physicsVertices[20*20*4];
};