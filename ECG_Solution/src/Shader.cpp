#include "Shader.h"

void Shader::activate() {
    glUseProgram(shader);
    if (diffuseTexture != 0) {
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
        glUniform1i(getUniformLocation("diffuseTexture"), 0);
    }
    if (specularTexture != 0) {
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, specularTexture);
        glUniform1i(getUniformLocation("specularTexture"), 1);
    }
}

Shader::~Shader() {
    glDeleteProgram(shader);
}

Shader::Shader(const string &vertexShaderPath, const string &fragmentShaderPath) {
    shader = glCreateProgram();

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderPath);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);
    glValidateProgram(shader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint Shader::compileShader(GLuint type, const std::string& path) const {
    std::ifstream stream(path);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string source = buffer.str();

    GLuint shaderId = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    GLint succeeded;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &succeeded);

    if (succeeded == GL_FALSE) {
        //Log auslesen und ausgeben
        GLint logSize;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
        char* message = new char[logSize];
        glGetShaderInfoLog(shaderId, logSize, nullptr, message);
        std::cerr << "Error in compileShader for " << path  << ": " << message << std::endl;
        delete[] message;
    }

    return shaderId;
}

void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) {
    activate();
    glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
}

void Shader::setUniform3f(const std::string& name, float v1, float v2, float v3) {
    activate();
    glUniform3f(getUniformLocation(name), v1, v2, v3);
}

void Shader::setUniform1f(const string &name, float v) {
    activate();
    glUniform1f(getUniformLocation(name), v);
}

void Shader::setUniform1i(const string &name, int v) {
    activate();
    glUniform1i(getUniformLocation(name), v);
}

void Shader::setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, glm::mat4 matrix) {
    setUniformMatrix4fv(name, size, transposed, &matrix[0][0]);
}

void Shader::setUniformMatrix4fv(const std::string &name, int size, GLboolean transposed, GLfloat* matrix) {
    activate();
    glUniformMatrix4fv(getUniformLocation(name), size, transposed, matrix);
}

void Shader::addUniformPointLight(const std::string& name, PointLight pointLight) {
    activate();
    GLint loc = getUniformLocation(name + "[" + std::to_string(pointLights) + "].position");
    glUniform3f(loc, pointLight.getPosition().x, pointLight.getPosition().y, pointLight.getPosition().z);
    loc = getUniformLocation(name + "[" + std::to_string(pointLights) + "].intensity");
    glUniform3f(loc, pointLight.getIntensity().r, pointLight.getIntensity().g, pointLight.getIntensity().b);
    loc = getUniformLocation(name + "[" + std::to_string(pointLights) + "].attenuation");
    glUniform3f(loc, pointLight.getAttenuation().x, pointLight.getAttenuation().y, pointLight.getAttenuation().z);
    pointLights++;
    setUniform1i("amountOfPointLights", pointLights);
}

void Shader::addUniformDirectionalLight(const std::string& name, DirectionalLight directionalLight) {
    activate();
    GLint loc = getUniformLocation(name + "[" + std::to_string(directionalLights) + "].direction");
    glUniform3f(loc, directionalLight.getDirection().x, directionalLight.getDirection().y, directionalLight.getDirection().z);
    loc = getUniformLocation(name  + "[" + std::to_string(directionalLights) + "].intensity");
    glUniform3f(loc, directionalLight.getIntensity().r, directionalLight.getIntensity().g, directionalLight.getIntensity().b);
    directionalLights++;
    setUniform1i("amountOfDirectionalLights", directionalLights);
}

void Shader::addUniformSpotLight(const std::string& name, SpotLight spotLight) {
    activate();
    GLint loc = getUniformLocation(name + "[" + std::to_string(spotLights) + "].position");
    glUniform3f(loc, spotLight.getPosition().x, spotLight.getPosition().y, spotLight.getPosition().z);
    loc = getUniformLocation(name + "[" + std::to_string(spotLights) + "].direction");
    glUniform3f(loc, spotLight.getDirection().x, spotLight.getDirection().y, spotLight.getDirection().z);
    loc = getUniformLocation(name  + "[" + std::to_string(spotLights) + "].intensity");
    glUniform3f(loc, spotLight.getIntensity().r, spotLight.getIntensity().g, spotLight.getIntensity().b);
    loc = getUniformLocation(name + "[" + std::to_string(spotLights) + "].attenuation");
    glUniform3f(loc, spotLight.getAttenuation().x, spotLight.getAttenuation().y, spotLight.getAttenuation().z);
    loc = getUniformLocation(name + "[" + std::to_string(spotLights) + "].angle");
    glUniform1f(loc, spotLight.getAngle());
    spotLights++;
    setUniform1i("amountOfSpotLights", spotLights);
}

GLint Shader::getUniformLocation(const std::string& name) {
    if (uniformLocationMap.find(name) != uniformLocationMap.end()) {
        return uniformLocationMap[name];
    }
    GLint id = glGetUniformLocation(shader, name.c_str());
    uniformLocationMap[name] = id;
    return id;
}

void Shader::loadTexture(const std::string &texturePath, int unit) {
    DDSImage ddsImage = loadDDS(texturePath.c_str());
    if (unit == 0) {
        glGenTextures(1, &diffuseTexture);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    } else if (unit == 1) {
        glGenTextures(1, &specularTexture);
        glBindTexture(GL_TEXTURE_2D, specularTexture);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, ddsImage.format, ddsImage.width, ddsImage.height, 0, ddsImage.size, ddsImage.data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
