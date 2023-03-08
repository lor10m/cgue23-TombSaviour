#pragma once

#include <vector>
#include "Shader.h"
#include "Camera.h"

class ShaderManager {
private:
    const int MAX_LIGHTS = 16;
    std::vector<PointLight> pointLights = {};
    std::vector<DirectionalLight> directionalLights = {};
    std::vector<SpotLight> spotLights = {};
    std::vector<Shader*> shaders = {};
public:
    void updateCameraValues(Camera& camera);
    bool addPointLight(PointLight pointLight);
    bool addDirectionalLight(DirectionalLight directionalLight);
    bool addSpotLight(SpotLight spotLight);
    Shader* createGouraudShader(glm::vec4 color, float ka, float kd, float ks, int alpha);
    Shader* createPhongShader(glm::vec4 color, float ka, float kd, float ks, int alpha);
    Shader* createPhongShader(const std::string& diffuseTexture, const std::string& specularTexture, float ka, float kd, float ks, int alpha);
    Shader* createCookTorranceShader(glm::vec4 color, float ka, float kd, float roughness, float ior);
};