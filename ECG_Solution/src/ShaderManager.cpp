#include "ShaderManager.h"

void ShaderManager::updateCameraValues(Camera& camera) {
    for (auto * shader : shaders) {
        shader->setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera.getTransformMatrix());
        shader->setUniform3f("eyePos", camera.pos.x, camera.pos.y, camera.pos.z);
    }
}

bool ShaderManager::addPointLight(PointLight pointLight) {
    if (pointLights.size() == MAX_LIGHTS) {
        return false;
    }
    pointLights.push_back(pointLight);
    return true;
}

bool ShaderManager::addDirectionalLight(DirectionalLight directionalLight) {
    if (directionalLights.size() == MAX_LIGHTS) {
        return false;
    }
    directionalLights.push_back(directionalLight);
    return true;
}

bool ShaderManager::addSpotLight(SpotLight spotLight) {
    if (spotLights.size() == MAX_LIGHTS) {
        return false;
    }
    spotLights.push_back(spotLight);
    return true;
}

Shader* ShaderManager::createGouraudShader(glm::vec4 color, float ka, float kd, float ks, int alpha) {
    Shader* shader = new Shader("assets/shaders/gouraud.vsh", "assets/shaders/gouraud.fsh");
    shader->activate();
    shader->setUniform4f("inColor", color.r, color.g, color.b, color.a);
    shader->setUniform1f("ka", ka);
    shader->setUniform1f("kd", kd);
    shader->setUniform1f("ks", ks);
    shader->setUniform1i("alpha", alpha);

    for (const auto & pointLight : pointLights) {
        shader->addUniformPointLight("pointLight", pointLight);
    }

    for (const auto & directionalLight : directionalLights) {
        shader->addUniformDirectionalLight("directionalLight", directionalLight);
    }

    for (const auto & spotLight : spotLights) {
        shader->addUniformSpotLight("spotLight", spotLight);
    }

    shaders.push_back(shader);
    return shader;
}

Shader* ShaderManager::createPhongShader(glm::vec4 color, float ka, float kd, float ks, int alpha) {
    Shader* shader = new Shader("assets/shaders/phong.vsh", "assets/shaders/phong.fsh");
    shader->activate();
    shader->setUniform4f("inColor", color.r, color.g, color.b, color.a);
    shader->setUniform1f("ka", ka);
    shader->setUniform1f("kd", kd);
    shader->setUniform1f("ks", ks);
    shader->setUniform1i("alpha", alpha);

    for (const auto & pointLight : pointLights) {
        shader->addUniformPointLight("pointLight", pointLight);
    }

    for (const auto & directionalLight : directionalLights) {
        shader->addUniformDirectionalLight("directionalLight", directionalLight);
    }

    for (const auto & spotLight : spotLights) {
        shader->addUniformSpotLight("spotLight", spotLight);
    }

    shaders.push_back(shader);
    return shader;
}

Shader* ShaderManager::createCookTorranceShader(glm::vec4 color, float ka, float kd, float roughness, float ior) {
    Shader* shader = new Shader("assets/shaders/cookTorrance.vsh", "assets/shaders/cookTorrance.fsh");
    shader->activate();
    shader->setUniform4f("inColor", color.r, color.g, color.b, color.a);
    shader->setUniform1f("ka", ka);
    shader->setUniform1f("kd", kd);
    shader->setUniform1f("m", roughness);
    shader->setUniform1f("n", ior);

    for (const auto & pointLight : pointLights) {
        shader->addUniformPointLight("pointLight", pointLight);
    }

    for (const auto & directionalLight : directionalLights) {
        shader->addUniformDirectionalLight("directionalLight", directionalLight);
    }

    for (const auto & spotLight : spotLights) {
        shader->addUniformSpotLight("spotLight", spotLight);
    }

    shaders.push_back(shader);
    return shader;
}

Shader *ShaderManager::createPhongShader(const std::string& diffuseTexture, const std::string& specularTexture, float ka, float kd, float ks, int alpha) {
    Shader* shader = new Shader("assets/shaders/phong.vsh", "assets/shaders/phong.fsh");
    shader->activate();
    shader->loadTexture(diffuseTexture, 0);
    shader->loadTexture(specularTexture, 1);
    shader->setUniform1f("ka", ka);
    shader->setUniform1f("kd", kd);
    shader->setUniform1f("ks", ks);
    shader->setUniform1i("alpha", alpha);

    for (const auto & pointLight : pointLights) {
        shader->addUniformPointLight("pointLight", pointLight);
    }

    for (const auto & directionalLight : directionalLights) {
        shader->addUniformDirectionalLight("directionalLight", directionalLight);
    }

    for (const auto & spotLight : spotLights) {
        shader->addUniformSpotLight("spotLight", spotLight);
    }

    shaders.push_back(shader);
    return shader;
}
