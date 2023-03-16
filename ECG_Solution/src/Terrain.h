#pragma once

#include "Utils.h"
#include <vector>
#include "stb_image.h"
#include "TerrainShader.h"

class Terrain {

private:
	void genHeatMap(TerrainShader tessHeightMapShader);
	void genTerrainTexture(TerrainShader tessHeightMapShader, const std::string& texturePath);
	unsigned int heightMapTexture;
	unsigned int surfaceTexture;
	std::vector<float> vertices;
	unsigned int terrainVAO, terrainVBO;
	const unsigned int NUM_PATCH_PTS = 4;
	unsigned rez = 20;
	int width, height, nrChannels;

public:
	Terrain(TerrainShader tessHeightMapShader, const std::string& texturePath);
	void render();
	void deleteTerrain();
};