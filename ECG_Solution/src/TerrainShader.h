#pragma once

#include "Utils.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class TerrainShader
{
public:
	GLuint ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	TerrainShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath,
		const char* tessControlPath, const char* tessEvalPath);
	TerrainShader();
	// activate the shader
	// ------------------------------------------------------------------------
	void use();

	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value);

	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value);

	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value);

	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value);

	void setVec2(const std::string& name, float x, float y);

	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value);

	void setVec3(const std::string& name, float x, float y, float z);

	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value);

	void setVec4(const std::string& name, float x, float y, float z, float w);

	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat);

	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat);

	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat);


private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(GLuint shader, std::string type);

	const char* readFile(const char* path);
};