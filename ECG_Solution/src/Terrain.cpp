#include "Terrain.h"

Terrain::Terrain(){}

void Terrain::createTerrain(const char* texturePath, const char* heightmapPath)
{
	cubeShader.createPhongShader("assets/textures/tiles_diffuse.dds", "assets/textures/tiles_specular.dds", true, glm::mat4(1.0f), 0.0f, 0.0f, 0.0f, 0);
	cube.generateModel("assets/objects/cube2.obj");

	shader.createTerrainShader("", "");
	shader.activate();
	diffuseTexture.genTexture("assets/textures/brick.jpg");
	specularTexture.genTexture("assets/textures/brick_specular.jpg");
	heightmapTexture.genTexture(heightmapPath);

	depthShader.createDepthMapShader();

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

	std::cout << "Loaded " << rez * rez << " patches of 4 control points each" << std::endl;
	std::cout << "Processing " << rez * rez * 4 << " vertices in vertex shader" << std::endl;
	
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

	glPatchParameteri(GL_PATCH_VERTICES, NUM_PATCH_PTS); //TODO


	// configure depth map FBO
	// -----------------------
	glGenFramebuffers(1, &depthMapFBO);

	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




}

void Terrain::render(glm::mat4 viewMatrix, glm::vec3 eyePos)
{
	glm::mat4 cubeModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 15.0f, 2.0f));

	//test += 0.2;
	//std::cout << test << std::endl;

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); //nicht verwendet im shader
	glm::vec3 lightPos = glm::vec3(-1.2f, 16.0f, 2.0f);
	glm::mat4 orthgonalProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 1.0f, 1077.5f);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = orthgonalProjection * lightView;

	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	// Draw model/mesh with shadowMap shader
	depthShader.setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);
	depthShader.setUniformMatrix4fv("model", 1, GL_FALSE, glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 10.0, 2.0f)));
	cube.draw(&cubeShader);

	depthShader.setUniformMatrix4fv("model", 1, GL_FALSE, glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0)));
	//draw terrain depth
	glBindVertexArray(terrainVAO);
	glDrawArrays(GL_PATCHES, 0, NUM_PATCH_PTS * rez * rez);


	// Switch back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, 800, 800);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------
	
	//CUBE
	cubeShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, viewMatrix);
	cubeShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, cubeModelMatrix);
	cubeShader.setUniform3f("eyePos", eyePos.x, eyePos.y, eyePos.z);
	cubeShader.setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	cubeShader.setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	cubeShader.setUniform1i("shadowMap", 2);

	cube.draw(&cubeShader);



	//TERRAIN
	shader.setUniformMatrix4fv("view", 1, GL_FALSE, viewMatrix);
	shader.setUniformMatrix4fv("model", 1, GL_FALSE, glm::mat4(1.0f));
	shader.setUniform3f("eyePos", eyePos.x, eyePos.y, eyePos.z); //y + 10
	shader.setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	shader.setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);

	heightmapTexture.bind(0);
	shader.setUniform1i("heightMap", 0);

	diffuseTexture.bind(1);
	shader.setUniform1i("diffuseTexture", 1);

	specularTexture.bind(2);
	shader.setUniform1i("specularTexture", 2);

	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	shader.setUniform1i("shadowMap", 3);

	glBindVertexArray(terrainVAO);
	glDrawArrays(GL_PATCHES, 0, NUM_PATCH_PTS * rez * rez);
}


void Terrain::deleteTerrain() {
	glDeleteVertexArrays(1, &terrainVAO);
	glDeleteBuffers(1, &terrainVBO);
}
