#include "Terrain.h"

Terrain::Terrain(){}

void Terrain::createTerrain(const char* texturePath, const char* heightmapPath)
{
	//cubeShader.createPhongShader("assets/textures/tiles_diffuse.dds", "assets/textures/tiles_specular.dds", "", true, glm::mat4(1.0f), 0.0f, 0.0f, 0.0f, 0);
	//cube.generateModel("assets/objects/cube2.obj");

	//pyramid.generateModel("assets/objects/cube2.obj");

	shader.createTerrainShader("", "");
	shader.activate();
	diffuseTexture.genTexture("assets/textures/brick.jpg");
	specularTexture.genTexture("assets/textures/brick_specular.jpg");
	heightmapTexture.genTexture(heightmapPath);

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


	//// configure depth map FBO
	//// -----------------------
	//glGenFramebuffers(1, &depthMapFBO);

	//// create depth texture
	//glGenTextures(1, &depthMap);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //GL_CLAMP_TO_BORDER //GL_REPEAT
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//// attach depth texture as FBO's depth buffer
	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Terrain::render(glm::mat4 viewMatrix, glm::vec3 eyePos, glm::vec3 lightPos, glm::mat4 lightSpaceMatrix, unsigned int depthMap)
{
	//sieht man von anfang an ab + 25 wirds dunkel 
	//test += 0.01;

	//glm::vec3 lightPos = glm::vec3(0.5f, 2.0f, 2.0f);
	//glm::vec3 lightDir = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::mat4 orthgonalProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
	//glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), lightDir);
	//glm::mat4 lightSpaceMatrix = orthgonalProjection * lightView;	
	
	//glm::vec3 lightPos = glm::vec3(-2.0f, 4, -1);
	//glm::mat4 depthProjectionMat = glm::ortho<float>(-10, 10, -10, 10, 1.0f, 7.5f);
	//glm::mat4 depthViewMat = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0, 1, 0));
	//glm::mat4 depthModelMat = glm::mat4(1.0f);

	//glm::mat4 lightSpaceMatrix = depthProjectionMat * depthViewMat * depthModelMat;



	//glm::mat4 model2 = glm::mat4(1.0f);
	//model2 = glm::translate(model2, glm::vec3(0.0, 1.0 , 0.0));
	//model2 = glm::scale(model2, glm::vec3(1.0f));


	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	//glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	//// Draw model/mesh with shadowMap shader
	//depthShader.setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix); //depthShader.setUniformMatrix4fv("view", 1, GL_FALSE, lightSpaceMatrix);

	////draw cube depth
	//depthShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, model2);
	//pyramid.draw(&cubeShader);

	//depthShader.setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);
	//glm::mat4 model3 = glm::mat4(1.0f);
	//model3 = glm::translate(model3, glm::vec3(0.0f, 0.0, 0.0));
	//depthShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, model3);


	// Switch back to the default framebuffer
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//// reset viewport
	//glViewport(0, 0, 800, 800);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	



	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------
	
	//debugShader.setUniform1f("near_plane", -10.0f);
	//debugShader.setUniform1f("far_plane", 20.0f);
	//debugShader.setUniform1i("depthMap", 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	//renderDebug();
	
	//TERRAIN
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

	if (depthMap != -1) {
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shader.setUniform1i("shadowMap", 3);
	}
	glBindVertexArray(terrainVAO);
	glDrawArrays(GL_PATCHES, 0, NUM_PATCH_PTS * rez * rez);


	////CUBE
	//cubeShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, viewMatrix);
	//cubeShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, glm::translate(model2, glm::vec3(0.0, 35.0, 0.0)));
	//cubeShader.setUniform3f("eyePos", eyePos.x, eyePos.y, eyePos.z);
	//cubeShader.setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	//cubeShader.setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);

	//glActiveTexture(GL_TEXTURE0 + 2);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	//cubeShader.setUniform1i("shadowMap", 2);

	//pyramid.draw(&cubeShader);

	////cubeShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, model2);
	////pyramid.draw(&cubeShader);
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