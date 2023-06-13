#include "Shader.h"
#include "Utils/stb_image.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <filesystem>

using namespace cv;
using namespace std;


Shader::Shader() {}

void Shader::createPhongShader(const std::string& diffuseTexture, const std::string& specularTexture, glm::mat4 modelMatrix, float ka, float kd, float ks, int alpha) {
	shader = glCreateProgram();

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "assets/shaders/phong.vsh");
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "assets/shaders/phong.fs");

	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);
	glValidateProgram(shader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	loadDDSTexture(diffuseTexture, 0);
	loadDDSTexture(specularTexture, 1);
	setUniform1f("ka", ka);
	setUniform1f("kd", kd);
	setUniform1f("ks", ks);
	setUniform1i("alpha", alpha);
	setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, modelMatrix);

	//TODO: lights
	PointLight pointLight1({ 0, 50, 0 }, { 100, 100, 100 }, { 1.0f, 0.4f, 0.1f });
	DirectionalLight directionalLight1({ 0, -1, -1 }, { 0.8f, 0.8f, 0.8f });
	addUniformPointLight("pointLight", pointLight1);
	addUniformDirectionalLight("directionalLight", directionalLight1);
}

void Shader::createNormalShader(const char* diffuseTexturePath, const char* specularTexturePath, const char* normalTexturePath, glm::mat4 modelMatrix)
{
	shader = glCreateProgram();

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "assets/shaders/normal.vsh");
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "assets/shaders/normal.fs");


	loadTexture(diffuseTexturePath, 0);
	loadTexture(specularTexturePath, 1);
	loadTexture(normalTexturePath, 2);

	setUniform1i("diffuseTexture", 0);
	setUniform1i("specularTexture", 1);
	setUniform1i("normalTexture", 2);

	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);
	glValidateProgram(shader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::createPhongShader(glm::mat4 modelMatrix, float ka, float kd, float ks, int alpha) {
	shader = glCreateProgram();

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "assets/shaders/phong.vsh");
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "assets/shaders/phong.fs");

	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);
	glValidateProgram(shader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	setUniform1f("ka", ka);
	setUniform1f("kd", kd);
	setUniform1f("ks", ks);
	setUniform1i("alpha", alpha);
	setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, modelMatrix);

	//TODO lights
	//PointLight pointLight1({ 0, 50, 0 }, { 2, 2, 2}, {1.0f, 0.4f, 0.1f});
	DirectionalLight directionalLight1({ 0, -1, 0 }, { 1.5f, 1.5f, 1.5f });
	//addUniformPointLight("pointLight", pointLight1);
	addUniformDirectionalLight("directionalLight", directionalLight1);
}

void Shader::createPhongVideoTexShader(const std::string& videoPath, glm::mat4 modelMatrix, float ka, float kd, float ks, int alpha) {
	shader = glCreateProgram();

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "assets/shaders/phong.vsh");
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "assets/shaders/phong.fs");

	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);
	glValidateProgram(shader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	loadVideoTexture(videoPath, 0);
	setUniform1f("ka", ka);
	setUniform1f("kd", kd);
	setUniform1f("ks", ks);
	setUniform1i("alpha", alpha);
	setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, modelMatrix);

	//TODO: lights
	PointLight pointLight1({ 0, 50, 0 }, { 100, 100, 100 }, { 1.0f, 0.4f, 0.1f });
	DirectionalLight directionalLight1({ 0, -1, -1 }, { 0.8f, 0.8f, 0.8f });
	addUniformPointLight("pointLight", pointLight1);
	addUniformDirectionalLight("directionalLight", directionalLight1);
}

//void Shader::loadDDSTexture(const std::string& texturePath, int unit) {
//	DDSImage ddsImage = loadDDS(texturePath.c_str());
//
//	//std::cout << "textureSize of texture: " << texturePath << " size: " << ddsImage.height << ", " << ddsImage.width;
//	if (unit == 0) {
//		glGenTextures(1, &diffuseTexture);
//		glBindTexture(GL_TEXTURE_2D, diffuseTexture);
//	}
//	else if (unit == 1) {
//		glGenTextures(1, &specularTexture);
//		glBindTexture(GL_TEXTURE_2D, specularTexture);
//	}
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glCompressedTexImage2D(GL_TEXTURE_2D, 0, ddsImage.format, ddsImage.width, ddsImage.height, 0, ddsImage.size, ddsImage.data);
//	glGenerateMipmap(GL_TEXTURE_2D);
//}

void Shader::loadVideoTexture(const std::string& videoTexturePath, int unit) {

	std::cout << "\nOpenCV version is: " << CV_VERSION << "**************\n" << endl;

	std::cout << videoTexturePath;
	VideoCapture video(videoTexturePath);
	
	if (!video.isOpened()) {
		return;
		std::cout << "No video";
	}

	cv::Mat frame;
	while (video.read(frame)) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format;
		GLenum type;
		if (frame.channels() == 3) {
			format = GL_BGR;
			type = GL_UNSIGNED_BYTE;
		}
		else if (frame.channels() == 4) {
			format = GL_BGRA;
			type = GL_UNSIGNED_BYTE;
		}
		else {
			// Ungültiges Bildformat
			glDeleteTextures(1, &texture);
			continue;
		}

		// Bild auf die OpenGL-Textur laden
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, format, type, frame.data);
		glGenerateMipmap(GL_TEXTURE_2D);

		videoTextures.push_back(texture);
	}
}

void Shader::createTerrainShader()
{
	shader = glCreateProgram();
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "assets/shaders/terrainVertex.vs");
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "assets/shaders/terrainFragment.fs");
	unsigned int tessControlShader = compileShader(GL_TESS_CONTROL_SHADER, "assets/shaders/terrain.tcs");
	unsigned int tessEvalShader = compileShader(GL_TESS_EVALUATION_SHADER, "assets/shaders/terrain.tes");

	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glAttachShader(shader, tessControlShader);
	glAttachShader(shader, tessEvalShader);
	glLinkProgram(shader);
	glValidateProgram(shader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(tessControlShader);
	glDeleteShader(tessEvalShader);
}

void Shader::createSimpleShader(glm::vec3 color, glm::mat4 modelMatrix)
{
	shader = glCreateProgram();
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "assets/shaders/vertex.shader");
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "assets/shaders/fragment.shader");

	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);
	glValidateProgram(shader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	setUniform3f("inColor", color.x, color.y, color.z);
	setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, modelMatrix);
}

void Shader::createHDUShader(const std::string& texturePath)
{
	shader = glCreateProgram();

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "assets/shaders/guiShader.vsh");
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "assets/shaders/guiShader.fs");

	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);
	glValidateProgram(shader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	loadDDSTexture(texturePath, 0);
}

void Shader::changeTexture(const std::string& newTexturePath)
{
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind previous texture

	loadHUDTexture(newTexturePath, 0); // load + bind new texture
}

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
	if (normalTexture != 0) {
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, specularTexture);
		glUniform1i(getUniformLocation("normalTexture"), 2);
	}

	if (videoTextures.size() > 0) {
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, videoTextures[20]);
		glUniform1i(getUniformLocation("diffuseTexture"), 0);
	}

}

Shader::~Shader() {
	glDeleteProgram(shader);
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
		std::cerr << "Error in compileShader for " << path << ": " << message << std::endl;
		delete[] message;
	}

	return shaderId;
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
	loc = getUniformLocation(name + "[" + std::to_string(directionalLights) + "].intensity");
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
	loc = getUniformLocation(name + "[" + std::to_string(spotLights) + "].intensity");
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

void Shader::loadDDSTexture(const std::string& texturePath, int unit) {
	DDSImage ddsImage = loadDDS(texturePath.c_str());

	//std::cout << "textureSize of texture: " << texturePath << " size: " << ddsImage.height << ", " << ddsImage.width;
	if (unit == 0) {
		glGenTextures(1, &diffuseTexture);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	}
	else if (unit == 1) {
		glGenTextures(1, &specularTexture);
		glBindTexture(GL_TEXTURE_2D, specularTexture);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, ddsImage.format, ddsImage.width, ddsImage.height, 0, ddsImage.size, ddsImage.data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Shader::loadHUDTexture(const std::string& texturePath, int unit) {
	DDSImage ddsImage = loadDDS(texturePath.c_str());

	//std::cout << "textureSize of texture: " << texturePath << " size: " << ddsImage.height << ", " << ddsImage.width;
	if (unit == 0) {
		glGenTextures(1, &diffuseTexture);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	}
	else if (unit == 1) {
		glGenTextures(1, &specularTexture);
		glBindTexture(GL_TEXTURE_2D, specularTexture);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, ddsImage.format, ddsImage.width, ddsImage.height, 0, ddsImage.size, ddsImage.data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Shader::loadTexture(const char* texturePath, int unit) 
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	if (unit == 0) {
		glGenTextures(1, &diffuseTexture);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	}
	else if (unit == 1) {
		glGenTextures(1, &specularTexture);
		glBindTexture(GL_TEXTURE_2D, specularTexture);
	}
	else if (unit == 2) {
		glGenTextures(1, &normalTexture);
		glBindTexture(GL_TEXTURE_2D, normalTexture);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) {
	activate();
	glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
}

void Shader::setUniform3f(const std::string& name, float v1, float v2, float v3) {
	activate();
	glUniform3f(getUniformLocation(name), v1, v2, v3);
}

void Shader::setUniform1f(const string& name, float v) {
	activate();
	glUniform1f(getUniformLocation(name), v);
}

void Shader::setUniform1i(const string& name, int v) {
	activate();
	glUniform1i(getUniformLocation(name), v);
}

void Shader::setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, glm::mat4 matrix) {
	activate();
	setUniformMatrix4fv(name, size, transposed, &matrix[0][0]);
}

void Shader::setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, GLfloat* matrix) {
	activate();
	glUniformMatrix4fv(getUniformLocation(name), size, transposed, matrix);
}

void Shader::changeShader(std::shared_ptr<Camera> camera)
{
	/*use();
	setUniform("viewProjectionMat", camera->getViewProjMat());
	setUniform("cameraWorld", camera->getWorldPosition());
	setUniform("brightness", GlobalManager::get().getBrightness());*/
}


void Shader::cleanupVideoTextures() {		// TODO: use this somewhere
	for (GLuint texture : videoTextures) {
		glDeleteTextures(1, &texture);
	}
	videoTextures.clear();
}

