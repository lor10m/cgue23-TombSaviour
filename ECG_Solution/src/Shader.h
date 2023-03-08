#pragma once

#include "Utils.h"
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
    std::unordered_map<std::string, GLint> uniformLocationMap;
public:
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();
    GLuint compileShader(GLuint type, const std::string& path) const;
    void activate();
    void loadTexture(const std::string& texturePath, int unit);
    void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
    void setUniform3f(const std::string& name, float v1, float v2, float v3);
    void setUniform1f(const std::string& name, float v);
    void setUniform1i(const std::string& name, int v);
    void setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, glm::mat4 matrix);
    void setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, GLfloat* matrix);
    void addUniformPointLight(const std::string& name, PointLight pointLight);
    void addUniformDirectionalLight(const std::string& name, DirectionalLight directionalLight);
    void addUniformSpotLight(const std::string& name, SpotLight spotLight);
    GLint getUniformLocation(const std::string& name);
};