#pragma once

#include "Utils/Utils.h"
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Camera.h"
#include "Lights/SpotLight.h"
#include <sstream>
#include <fstream>
#include <unordered_map>

class Shader {
private:
    GLuint shader;
    int pointLights = 0;
    int directionalLights = 0;
    int spotLights = 0;
    GLuint diffuseTexture = 0;
    GLuint specularTexture = 0;
    GLuint normalTexture = 0;
    std::unordered_map<std::string, GLint> uniformLocationMap;

    std::vector<GLuint> videoTextures;
    float elapsedTime = 0.0f;
    float desiredFrameRate = 25.0f; // desired frame rate => at least 24 per second
    float frameDuration = 1.0f / desiredFrameRate;
    int videoFrameCount;
    int countFrames = 0;
    int elapsedFrames = 0;
    int oldFrame = 0;
    bool oneBlocker = false;
    int crrFrameIndex = 0;

public:
    Shader();
    ~Shader();

    void createPhongShader(const std::string& diffuseTexture, const std::string& specularTexture, glm::mat4 modelMatrix, float ka, float kd, float ks, int alpha);

    void createNormalShader(const char* diffuseTexture, const char* specularTexture, const char* normalTexture, glm::mat4 modelMatrix);

    void createPhongShader(glm::mat4 modelMatrix, float ka, float kd, float ks, int alpha);
    void createPhongVideoTexShader(const std::string& videoPath, glm::mat4 modelMatrix, float ka, float kd, float ks, int alpha);
    void loadVideoTexture(const std::string& videoTexturePath, int unit);
    void createTerrainShader();
    void createSimpleShader(glm::vec3 color, glm::mat4 modelMatrix);
 
    GLuint compileShader(GLuint type, const std::string& path) const;
    void activate();
    void loadDDSTexture(const std::string& texturePath, int unit);
    void loadHUDTexture(const std::string& texturePath, int unit);
    void loadTexture(const char* texturePath, int unit);
    void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
    void setUniform3f(const std::string& name, float v1, float v2, float v3);
    void setUniform1f(const std::string& name, float v);
    void setUniform1i(const std::string& name, int v);
    void setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, glm::mat4 matrix);
    void setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, GLfloat* matrix);
    void createHDUShader(const std::string& texturePath);
    void changeTexture(const std::string& newTexturePath);
    void changeShader(std::shared_ptr<Camera> camera);
    void cleanupVideoTextures();
    void addUniformPointLight(const std::string& name, PointLight pointLight);
    void addUniformDirectionalLight(const std::string& name, DirectionalLight directionalLight);
    void addUniformSpotLight(const std::string& name, SpotLight spotLight);
    GLint getUniformLocation(const std::string& name);
    int getcurrentFrameIndex();
    void setVideoFrameCount(int videoFrameCnt);
    void setCurrentFrame(float currFrame);

    std::vector<GLuint>& getVideoTextures() {
        return videoTextures;
    }
};