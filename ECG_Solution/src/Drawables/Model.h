#pragma once

#include "..\Utils.h"
#include "../Texture.h"
#include "../Shader.h"
#include <cmath>
#include <Importer.hpp>
#include <assimp/postprocess.h>
#include <mesh.h>
#include <vector>
#include <scene.h>
#include <array>
#include <glm/gtx/quaternion.hpp>

struct BoneInfo
{
	int id;/*id is index in finalBoneMatrices*/
	glm::mat4 offset; /*offset matrix transforms vertex from model space to bone space*/
	glm::mat4 finalTransformation;/* = glm::mat4(0.0f);*/
};

struct BoneInfo2
{
	glm::mat4 offset; /*offset matrix transforms vertex from model space to bone space*/
	glm::mat4 finalTranformation;

	BoneInfo2(const glm::mat4 offsetMatrix) {
		offset = offsetMatrix;
		finalTranformation = glm::mat4(0.0f); /*glm::zero<glm::mat4>();*/
	}
};

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
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

		//assert(0);
	}
};

struct Mesh
{

	std::vector<Vertex> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<BoneData> boneData;
	std::vector<Texture> textures;
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
	const aiScene* scene = NULL;

	unsigned int numNodes;

	std::vector<Mesh> modelMeshes;
	int numVertices;
	int numIndices;

	std::vector<BoneInfo2> boneInfo;
	std::map<string, BoneInfo> boneInfoMap;
	int boneCounter = 0;

	std::vector<Texture> textures;

	aiMatrix4x4 globalInverseTransform;

	void generateModel(string path);
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

	std::map<string, glm::mat4> lastTransformation;
	std::map<string, glm::mat4> currTransformation;


public:

	Model(string path);
	
	void draw(Shader* shader);

	void getBoneTransforms(float timeInSeconds, glm::mat4 globalTransform, std::vector<glm::mat4>& transforms);

	float getAnimationDuration();
	float getTicksPerSecond();

	//indices, vertices and normals from all meshes
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	std::vector<glm::mat4> finalBoneMatrices;

	aiNode* getRootBoneNode(aiNode* node);

	glm::mat4 tr = glm::mat4(1.0f);
	glm::mat4 xu = glm::mat4(1.0f);

	float at = 0.0f;

	int ajaj = 0;

	glm::mat4 yh;
};