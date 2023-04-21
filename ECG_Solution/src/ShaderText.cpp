//
//#include "ShaderText.h"
//#include <glm/gtc/type_ptr.hpp>
//
//void ShaderText::activate() {
//    glUseProgram(shader);
//    if (diffuseTexture != 0) {
//        glActiveTexture(GL_TEXTURE0 + 0);
//        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
//        glUniform1i(getUniformLocation("diffuseTexture"), 0);
//    }
//    if (specularTexture != 0) {
//        glActiveTexture(GL_TEXTURE0 + 1);
//        glBindTexture(GL_TEXTURE_2D, specularTexture);
//        glUniform1i(getUniformLocation("specularTexture"), 1);
//    }
//}
//
//ShaderText::~ShaderText() {
//    glDeleteProgram(shader);
//}
//
//ShaderText::ShaderText(const string& vertexShaderPath, const string& fragmentShaderPath) {
//    shader = glCreateProgram();
//
//    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderPath);
//    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);
//
//    glAttachShader(shader, vertexShader);
//    glAttachShader(shader, fragmentShader);
//    glLinkProgram(shader);
//    glValidateProgram(shader);
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//}
//
//GLuint ShaderText::compileShader(GLuint type, const std::string& path) const {
//    std::ifstream stream(path);
//    std::stringstream buffer;
//    buffer << stream.rdbuf();
//    std::string source = buffer.str();
//
//    GLuint shaderId = glCreateShader(type);
//    const char* src = source.c_str();
//    glShaderSource(shaderId, 1, &src, nullptr);
//    glCompileShader(shaderId);
//
//    GLint succeeded;
//    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &succeeded);
//
//    if (succeeded == GL_FALSE) {
//        //Log auslesen und ausgeben
//        GLint logSize;
//        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
//        char* message = new char[logSize];
//        glGetShaderInfoLog(shaderId, logSize, nullptr, message);
//        std::cerr << "Error in compileShader for " << path << ": " << message << std::endl;
//        delete[] message;
//    }
//
//    return shaderId;
//}
//
//GLint ShaderText::getUniformLocation(const std::string& name) {
//    if (uniformLocationMap.find(name) != uniformLocationMap.end()) {
//        return uniformLocationMap[name];
//    }
//    GLint id = glGetUniformLocation(shader, name.c_str());
//    uniformLocationMap[name] = id;
//    return id;
//}
//
//void ShaderText::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) {
//    activate();
//    glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
//}
//
//void ShaderText::setUniform3f(const std::string& name, float v1, float v2, float v3) {
//    activate();
//    glUniform3f(getUniformLocation(name), v1, v2, v3);
//}
//
//void ShaderText::setUniform1f(const string& name, float v) {
//    activate();
//    glUniform1f(getUniformLocation(name), v);
//}
//
//void ShaderText::setUniform1i(const string& name, int v) {
//    activate();
//    glUniform1i(getUniformLocation(name), v);
//}
//
//void ShaderText::setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, glm::mat4 matrix) {
//    setUniformMatrix4fv(name, size, transposed, &matrix[0][0]);
//}
//
//void ShaderText::setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, GLfloat* matrix) {
//    activate();
//    glUniformMatrix4fv(getUniformLocation(name), size, transposed, matrix);
//}
//
//
//
//void ShaderText::use(){
//    glUseProgram(ID);
//}
//void ShaderText::setUniform(std::string uniform, const int i) {
//    glUniform1i(ShaderText::getUniformLocation(uniform), i);
//}
//void ShaderText::setUniform(std::string uniform, const glm::mat4& mat) {
//    glUniformMatrix4fv(getUniformLocation(uniform), 1, false, glm::value_ptr(mat));
//}
//void ShaderText::setUniform(std::string uniform, const glm::vec3& vec) {
//    glUniform3fv(getUniformLocation(uniform), 1, glm::value_ptr(vec));
//}
//void ShaderText::unuse() {
//    glUseProgram(0);
//}