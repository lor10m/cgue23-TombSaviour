#include "Terrain.h"

Terrain::Terrain(){}

void Terrain::createTerrain(const char* texturePath, const char* heightmapPath, float ka, float kd, float ks, int alpha)
{
	shader.createTerrainShader("", "", ka, kd, ks, alpha);
	shader.activate();
	diffuseTexture.genTexture("assets/textures/terrain.jpg");
	specularTexture.genTexture("assets/textures/terrain_spec.jpg");
	heightmapTexture.genDDSTexture(heightmapPath);

	//depthShader.createDepthMapShader();
	//debugShader.createDebugShadowShader();

	height = heightmapTexture.height;
	width = heightmapTexture.width;
	//std::cout << "terrainWidth: " << width << " terrainheight: " << height;

	//// set up vertex data (and buffer(s)) and configure vertex attributes
	//// ------------------------------------------------------------------

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

	//std::cout << "Loaded " << rez * rez << " patches of 4 control points each" << std::endl;
	//std::cout << "Processing " << rez * rez * 4 << " vertices in vertex shader" << std::endl;
	//
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

void Terrain::render(glm::mat4 viewMatrix, glm::vec3 eyePos, glm::vec3 lightPos, glm::mat4 lightSpaceMatrix, unsigned int depthMap)
{
	//
	shader.setUniformMatrix4fv("view", 1, GL_FALSE, viewMatrix);
	shader.setUniformMatrix4fv("model", 1, GL_FALSE,glm::mat4(1.0f));
	shader.setUniform3f("eyePos", eyePos.x, eyePos.y, eyePos.z); //y + 10
	shader.setUniform3f("lightPos",lightPos.x, lightPos.y, lightPos.z);
	shader.setUniform3f("lightDir",0,1,0);
	shader.setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);
	heightmapTexture.bind(0);
	shader.setUniform1i("heightMap", 0);
	diffuseTexture.bind(1);
	shader.setUniform1i("diffuseTexture", 1);
	specularTexture.bind(2);
	shader.setUniform1i("specularTexture", 2);
	shader.setUniform1i("withShadow", false);

	if (depthMap != -1) {
		shader.setUniform1i("shadow", 1);
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shader.setUniform1i("shadowMap", 3);
	}
	glBindVertexArray(terrainVAO);
	glDrawArrays(GL_PATCHES, 0, NUM_PATCH_PTS * rez * rez);
}


void Terrain::deleteTerrain() {
	glDeleteVertexArrays(1, &terrainVAO);
	glDeleteBuffers(1, &terrainVBO);
}

void Terrain::renderDebug()
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