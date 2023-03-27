#pragma once

#include "..\Utils.h"
#include <cmath>
#include <Importer.hpp>
#include <mesh.h>
#include <vector>
#include <scene.h>


class Model
{
private:
	std::vector<glm::vec2> uvs;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	GLuint NormalsVBO;
	int iboSize;
	int vertexCount;
	aiMesh* mesh;

public:

	Model(string path);
	void generateModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(const aiScene* scene);
	void draw();

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;

};