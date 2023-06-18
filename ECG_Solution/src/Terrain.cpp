#include "Terrain.h"

Terrain::Terrain(){}

Terrain::Terrain(Shader* tessHeightMapShader, const char* texturePath, const char* heightmapPath) {

	shader = tessHeightMapShader;

	shader->activate();
	diffuseTexture.genTexture("assets/textures/brick.jpg");
	specularTexture.genTexture("assets/textures/brick_specular.jpg");
	heightmapTexture.genTexture(heightmapPath);

	height = heightmapTexture.height;
	width = heightmapTexture.width;

	std::cout << "terrainWidth: " << width << " terrainheight: " << height;

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	for (unsigned i = 0; i <= rez - 1; i++)
	{
		for (unsigned j = 0; j <= rez - 1; j++)
		{
			vertices.push_back(-width / 2.0f + width * i / (float)rez); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * j / (float)rez); // v.z
			vertices.push_back(j / (float)rez); // v
			vertices.push_back(i / (float)rez); // u

			vertices.push_back(-width / 2.0f + width * (i + 1) / (float)rez); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * j / (float)rez); // v.z
			vertices.push_back(j / (float)rez); // v
			vertices.push_back((i + 1) / (float)rez); // u

			vertices.push_back(-width / 2.0f + width * i / (float)rez); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * (j + 1) / (float)rez); // v.z
			vertices.push_back((j + 1) / (float)rez); // v
			vertices.push_back(i / (float)rez); // u

			vertices.push_back(-width / 2.0f + width * (i + 1) / (float)rez); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * (j + 1) / (float)rez); // v.z
			vertices.push_back((j + 1) / (float)rez); // v
			vertices.push_back((i + 1) / (float)rez); // u
		}
	}
	//for (unsigned int i = 0; i < rez - 1; i++)
	//{
	//	for (unsigned int j = 0; j < rez - 1; j++)
	//	{
	//		unsigned int topLeft = i * rez + j;
	//		unsigned int topRight = (i + 1) * rez + j;
	//		unsigned int bottomLeft = i * rez + (j + 1);
	//		unsigned int bottomRight = (i + 1) * rez + (j + 1);

	//		// Triangle 1
	//		indices.push_back(topLeft);
	//		indices.push_back(bottomLeft);
	//		indices.push_back(topRight);

	//		// Triangle 2
	//		indices.push_back(topRight);
	//		indices.push_back(bottomLeft);
	//		indices.push_back(bottomRight);

	//		indexCount += 6;
	//	}
	//}
	std::cout << "Loaded " << rez * rez << " patches of 4 control points each" << std::endl;
	std::cout << "Processing " << rez * rez * 4 << " vertices in vertex shader" << std::endl;
	//std::cout << "indexCount: " << indexCount << " size: " << indices.size() << std::endl;
	
	// first, configure the cube's VAO (and terrainVBO)
	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

	glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glPatchParameteri(GL_PATCH_VERTICES, NUM_PATCH_PTS);





}

void Terrain::render() {

	shader->activate();

	heightmapTexture.bind(0);
	shader->setUniform1i("heightMap", 0);

	diffuseTexture.bind(1);
	shader->setUniform1i("diffuseTexture", 1);

	specularTexture.bind(2);
	shader->setUniform1i("specularTexture", 2);

	//if (depthMap != -1) {
	//	glActiveTexture(GL_TEXTURE0 + 2);
	//	glBindTexture(GL_TEXTURE_2D, depthMap);
	//	shader->setUniform1i("shadowMap", 2);
	//}

	glBindVertexArray(terrainVAO);
	glDrawArrays(GL_PATCHES, 0, NUM_PATCH_PTS * rez * rez);
}


void Terrain::deleteTerrain() {
	glDeleteVertexArrays(1, &terrainVAO);
	glDeleteBuffers(1, &terrainVBO);
}
