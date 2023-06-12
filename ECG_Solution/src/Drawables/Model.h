#pragma once

#include "../Utils/Utils.h"
#include "../Texture.h"
#include "../Shader.h"
#include <cmath>
#include <assimp/postprocess.h>
#include <vector>
#include <array>
#include <glm/gtx/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <assimp/anim.h>
#include <assimp/scene.h>


struct BoneInfo
{
	int id;
	glm::mat4 offset;
	glm::mat4 finalTransformation;
};

//TODO use existing Vertex struct
struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 tangents;
	glm::vec3 bitangents;
};

struct BoneData
{
	unsigned int boneIDs[4] = { 0,0,0,0 };
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	void addBoneData(int boneID, float weight) {
		for (int i = 0; i < 4; i++) {
			if (weights[i] == 0.0f) {
				boneIDs[i] = boneID;
				weights[i] = weight;
				return;
			}
		}
	}
};

struct Mesh
{

	std::vector<Vertex> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	//std::vector<glm::vec3> tangents;
	//std::vector<glm::vec3> bitangents;

	std::vector<BoneData> boneData;
	int baseVertexIndex;
	int baseIndicesIndex;
	float ambient = -1;
	float diffuse = -1;
	float specular = -1;
	float alpha = -1;

	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	GLuint NormalsVBO;
	GLuint bonesVBO;
	GLuint weightsVBO;
	GLuint texCoordsVBO;

};

class Model
{
private:


	Assimp::Importer importer;
	Assimp::Importer importer1;
	const aiScene* scene = NULL;
	const aiScene* scene1 = NULL;


	unsigned int boneCounter = 0;
	unsigned int numNodes;
	unsigned int numVertices;
	unsigned int numIndices;

	std::vector<Mesh> modelMeshes;
	std::map<string, BoneInfo> boneInfoMap;
	std::vector<Texture> textures;
	aiMatrix4x4 globalInverseTransform;
	std::vector<glm::mat4> finalBoneMatrices;


	void processNode(aiNode* node);
	void processMesh(aiMesh* mesh, unsigned int meshIndex);
	void processBones(aiMesh* mesh, unsigned int meshIndex);
	void initBuffer();

	void initMaterials(aiMesh* mesh, unsigned int meshIndex);
	void loadTextures(unsigned int meshIndex, aiMaterial* material, enum aiTextureType type, string typeName);

	void readNodeHierachy(float animationTimeTicks, const aiNode* node, glm::mat4& parentTransform);

	const aiNodeAnim* findNodeAnim(const aiAnimation* animation, const string& nodeName);
	glm::mat4 CalcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	glm::mat4 CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	glm::mat4 CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

	glm::mat4 convertAiMatrixToGlm(const aiMatrix4x4& from);


public:

	const aiAnimation* animation = NULL;
	bool isDeadEnemy = false;

	//indices, vertices and normals from all meshes
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> vertices;

	glm::vec3 modelSize;

	glm::vec3 physxTransform = glm::vec3(0.0f);
	glm::mat4 physxRotate = glm::mat4(1.0f);

	Model();
	Model(string path);
	void generateModel(string path);
	void draw(Shader* shader);
	void getBoneTransforms(float timeInSeconds, glm::mat4 globalTransform, std::vector<glm::mat4>& transforms);
	float getAnimationDuration();
	float getTicksPerSecond();
};