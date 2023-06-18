#pragma once

#include "Utils/Utils.h"
#include "Shader.h"
#include "Utils/stb_image.h"

class ShadowMap {

private:
    Shader shader;
    Shader depthShader;
    Shader debugShader;

    unsigned int planeVAO;
    unsigned int planeVBO;

    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;

    unsigned int SHADOW_WIDTH = 2048;  //1024
    unsigned int SHADOW_HEIGHT = 2048;
    unsigned int depthMapFBO;
    unsigned int depthMap;

    unsigned int diffuseTexture;
    unsigned int specularTexture;

    glm::vec3 lightPos;
    glm::vec4 lightColor;
    glm::mat4 lightSpaceMatrix;

    void createPlane();

public:
    ShadowMap() {};

    void create();

    void render(glm::mat4 viewMatrix, glm::vec3 eyePos);

    void renderScene(Shader& shader, bool cull);

    void renderCube();

    void renderDebug();
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
};