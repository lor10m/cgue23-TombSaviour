#pragma once

#include "Utils/Utils.h"
#include "Texture.h"
#include "Shader.h"
#include "Terrain.h"

class DepthMap
{

private:

	GLuint depthMapFBO;
	GLuint depthMap;
	
	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;

	glm::vec3 lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);

public:

	DepthMap();
	void createTerrainDepthMap();
	void render(Shader* simpleDepthMapShader, Shader* terrainShader, Terrain* terrain, glm::mat4 viewMatrix, glm::mat4 projection);
};