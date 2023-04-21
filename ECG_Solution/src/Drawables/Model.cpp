#include "Model.h"
#include <postprocess.h>


#define POSITION_LOCATION    0
#define NORMAL_LOCATION      1
#define TEX_COORD_LOCATION   2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

Model::Model() {
}

void Model::generateModel(string path)
{
	scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs); //TODO
	std::cout << "Loading assimp!" << std::endl;

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
	}
	else {
		modelMeshes.resize(scene->mNumMeshes);
		numVertices = 0;
		numIndices = 0;
		int numBones = 0;
		for (int i = 0; i < scene->mNumMeshes; i++) {
			const aiMesh* mesh = scene->mMeshes[i];
			modelMeshes[i].baseVertexIndex = numVertices;
			modelMeshes[i].baseIndicesIndex = numIndices;
			numVertices += scene->mMeshes[i]->mNumVertices;
			numIndices += scene->mMeshes[i]->mNumFaces * 3;
			numBones += mesh->mNumBones;

		}

		aiNode* node = scene->mRootNode;

		finalBoneMatrices.resize(numBones);

		globalInverseTransform = scene->mRootNode->mTransformation;
		globalInverseTransform = globalInverseTransform.Inverse();

		processNode(node);
		initBuffer();

		std::cout << "numMeshes:" << scene->mNumMeshes << " numVertices: " << numVertices << " numIndices: " << numIndices << " numBones: " << numBones << " numAnims: " << scene->mNumAnimations << std::endl;
		std::cout << std::endl;

	}
}

void Model::processNode(aiNode* node)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		processMesh(scene->mMeshes[node->mMeshes[i]], i);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i]);
	}
}

void Model::processMesh(aiMesh* mesh, unsigned int meshIndex)
{
	modelMeshes[meshIndex].vertices.reserve(numVertices);
	modelMeshes[meshIndex].normals.reserve(numVertices);
	modelMeshes[meshIndex].indices.reserve(numIndices);
	modelMeshes[meshIndex].boneData.resize(numVertices);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D& pos = mesh->mVertices[i];
		Vertex vert;
		vert.position = glm::vec3(pos.x, pos.y, pos.z);
		vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));

		if (mesh->mNormals) {
			aiVector3D& normal = mesh->mNormals[i];
			modelMeshes[meshIndex].normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
			normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
		}
		else {
			modelMeshes[meshIndex].normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (mesh->mTextureCoords[0]) {
			aiVector3D text = mesh->mTextureCoords[0][i];
			vert.uv = glm::vec2(text.x, text.y);
		}
		else {
			vert.uv = glm::vec2(0.0f, 0.0f);
		}
		modelMeshes[meshIndex].vertices.push_back(vert);
	}

	initMaterials(mesh, meshIndex);

	processBones(mesh, meshIndex);

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			modelMeshes[meshIndex].indices.push_back(face.mIndices[j]);
			indices.push_back(face.mIndices[j]);
		}
	}
}


void Model::initMaterials(aiMesh* mesh, unsigned int meshIndex)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	loadTextures(meshIndex, material, aiTextureType_DIFFUSE, "diffuseTexture");
	loadTextures(meshIndex, material, aiTextureType_SPECULAR, "specularTexture");
}

void Model::loadTextures(unsigned int meshIndex, aiMaterial* material, enum aiTextureType type, string typeName)
{
	if (material->GetTextureCount(type) > 0) {
		aiColor4D ambientColor;
		aiColor4D diffuseColor;
		aiColor4D specularColor;

		material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
		material->Get(AI_MATKEY_OPACITY, modelMeshes[meshIndex].alpha);
		modelMeshes[meshIndex].ambient = ambientColor.r;
		modelMeshes[meshIndex].diffuse = diffuseColor.r;
		modelMeshes[meshIndex].specular = specularColor.r;
	}

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString str;
		material->GetTexture(type, i, &str);
		bool alreadyLoaded = false;
		std::string x = "assets/" + std::string(str.C_Str());

		for (unsigned int j = 0; j < textures.size(); j++) {
			if (textures[j].path == ("assets/" + std::string(str.C_Str()))) {
				modelMeshes[meshIndex].textures.push_back(textures[j]);
				textures.push_back(textures[j]);
				alreadyLoaded = true;
				break;
			}
		}
		if (!alreadyLoaded) {
			Texture text(x.c_str());
			text.type = typeName;
			modelMeshes[meshIndex].textures.push_back(text);
			textures.push_back(text);
		}
	}
}

void Model::processBones(aiMesh* mesh, unsigned int meshIndex)
{
	modelMeshes[meshIndex].boneData.reserve(modelMeshes[meshIndex].baseVertexIndex + numVertices);

	for (int i = 0; i < mesh->mNumBones; i++) {

		aiBone* bone = mesh->mBones[i];
		int boneID = -1;
		std::string boneName = bone->mName.C_Str();

		if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
			BoneInfo newBoneInfo;
			boneID = (int)boneInfoMap.size();
			newBoneInfo.id = boneID;
			newBoneInfo.offset = convertAiMatrixToGlm(bone->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
		}
		else {
			boneID = boneInfoMap[boneName].id;
		}

		auto weights = bone->mWeights;

		for (int j = 0; j < bone->mNumWeights; j++) {

			int vertexId = weights[j].mVertexId;
			float weight = weights[j].mWeight;
			int globalVertexID = modelMeshes[meshIndex].baseVertexIndex + vertexId;
			modelMeshes[meshIndex].boneData[globalVertexID].addBoneData(boneID, weight);

			modelMeshes[meshIndex].boneData[globalVertexID].addBoneData(boneInfoMap[boneName].id, weight);
		}
	}
}

void Model::getBoneTransforms(float timeInSeconds, glm::mat4 globalTransform, std::vector<glm::mat4>& transforms)
{

	transforms.resize(finalBoneMatrices.size());

	if (scene->mNumAnimations > 0) {
		float ticksPerSecond = scene->mAnimations[0]->mTicksPerSecond;
		float timeInTicks = timeInSeconds * ticksPerSecond;
		float animationTimeTicks = fmod(timeInTicks, scene->mAnimations[0]->mDuration);

		readNodeHierachy(animationTimeTicks, scene->mRootNode, glm::mat4(1.0f));

		for (unsigned int i = 0; i < finalBoneMatrices.size(); i++) {
			transforms[i] = finalBoneMatrices[i];
		}
	}
	else {
		for (unsigned int i = 0; i < finalBoneMatrices.size(); i++) {
			transforms[i] = glm::mat4(1.0f);
		}
	}

}

void Model::readNodeHierachy(float animationTimeTicks, const aiNode* node, glm::mat4& parentTransform)
{
	std::string nodeName = node->mName.data;
	glm::mat4 nodeTransform = convertAiMatrixToGlm(node->mTransformation);

	const aiAnimation* animation = scene->mAnimations[0];
	const aiNodeAnim* nodeAnim = findNodeAnim(animation, nodeName);

	if (nodeAnim) {
		glm::mat4 scale = CalcInterpolatedScaling(animationTimeTicks, nodeAnim);
		glm::mat4 rotation = CalcInterpolatedRotation(animationTimeTicks, nodeAnim);
		glm::mat4 translation = CalcInterpolatedPosition(animationTimeTicks, nodeAnim);
		nodeTransform = translation * rotation * scale;
	}

	glm::mat4 globalTransform = parentTransform * nodeTransform;

	if (node == scene->mRootNode->mChildren[1]) {
		glm::mat4 combined = glm::translate(glm::mat4(1.0f), physxTransform) * physxRotate;
		globalTransform = parentTransform * combined;
	}

	if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
		int index = boneInfoMap[nodeName].id;
		boneInfoMap[nodeName].finalTransformation = convertAiMatrixToGlm(globalInverseTransform) * globalTransform * boneInfoMap[nodeName].offset; //todo inverse
		finalBoneMatrices[index] = convertAiMatrixToGlm(globalInverseTransform) * globalTransform * boneInfoMap[nodeName].offset;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		readNodeHierachy(animationTimeTicks, node->mChildren[i], globalTransform);
	}

}

const aiNodeAnim* Model::findNodeAnim(const aiAnimation* animation, const string& nodeName)
{
	for (unsigned int i = 0; i < animation->mNumChannels; i++) {
		const aiNodeAnim* nodeAnim = animation->mChannels[i];

		if (nodeAnim->mNodeName.data == nodeName) {
			return nodeAnim;
		}
	}
	return NULL;
}

glm::mat4 Model::CalcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumScalingKeys == 1) {
		aiVector3D v = pNodeAnim->mScalingKeys[0].mValue;
		glm::vec3 vec = glm::vec3(v.x, v.y, v.z);
		return glm::scale(glm::mat4(1.0f), vec);
	}

	unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = ScalingIndex + 1;
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);

	float t1 = (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime;
	float t2 = (float)pNodeAnim->mScalingKeys[NextScalingIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTime - (float)t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	aiVector3D firstSc = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	glm::vec3 scale1 = glm::vec3(firstSc.x, firstSc.y, firstSc.z);
	aiVector3D secondSc = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	glm::vec3 scale2 = glm::vec3(secondSc.x, secondSc.y, secondSc.z);

	glm::vec3 finalScale = glm::mix(scale1, scale2, Factor);

	return glm::scale(glm::mat4(1.0f), finalScale);
}

glm::mat4 Model::CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		return glm::mat4(1.0f);
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = RotationIndex + 1;
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);

	float t1 = (float)pNodeAnim->mRotationKeys[RotationIndex].mTime;
	float t2 = (float)pNodeAnim->mRotationKeys[NextRotationIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTime - t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	aiQuaternion StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	glm::quat startrot(StartRotationQ.w, StartRotationQ.x, StartRotationQ.y, StartRotationQ.z);
	aiQuaternion EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	glm::quat endrot(EndRotationQ.w, EndRotationQ.x, EndRotationQ.y, EndRotationQ.z);

	glm::quat finalRotation = glm::slerp(startrot, endrot, Factor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Model::CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumPositionKeys == 1) {
		return glm::mat4(1.0f);
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = PositionIndex + 1;
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys); //140 pos keys

	float t1 = (float)pNodeAnim->mPositionKeys[PositionIndex].mTime;
	float t2 = (float)pNodeAnim->mPositionKeys[NextPositionIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTime - t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	glm::vec3 startp(Start.x, Start.y, Start.z);
	const aiVector3D End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	glm::vec3 endp(End.x, End.y, End.z);

	glm::vec3 finalPos = glm::mix(startp, endp, Factor);
	return glm::translate(glm::mat4(1.0f), finalPos);
}

unsigned int Model::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		float t = (float)pNodeAnim->mScalingKeys[i + 1].mTime;
		if (AnimationTime < t) {
			return i;
		}
	}

	return 0;
}

unsigned int Model::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		float t = (float)pNodeAnim->mRotationKeys[i + 1].mTime;
		if (AnimationTime < t) {
			return i;
		}
	}

	return 0;
}

unsigned int Model::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
		if (AnimationTime < t) {
			return i;
		}
	}

	return 0;
}

void Model::draw(Shader* shader)
{
	for (unsigned int i = 0; i < modelMeshes.size(); i++) {

		for (unsigned int i = 0; i < textures.size(); i++) {

			std::string name = textures[i].type;

			int x = 0;
			if (name == "specularTexture") x = 1;

			glActiveTexture(GL_TEXTURE0 + x);
			glBindTexture(GL_TEXTURE_2D, textures[i].handle);
			shader->setUniform1i(name, x);

		}

		glBindVertexArray(modelMeshes[i].vao);

		glDrawElements(GL_TRIANGLES, modelMeshes[i].indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

}

float Model::getAnimationDuration()
{
	return scene->mAnimations[0]->mDuration;
}

float Model::getTicksPerSecond()
{
	return scene->mAnimations[0]->mTicksPerSecond;
}

glm::mat4 Model::convertAiMatrixToGlm(const aiMatrix4x4& from)
{
	glm::mat4 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}


void Model::initBuffer()
{
	for (unsigned int i = 0; i < modelMeshes.size(); i++) {
		glGenVertexArrays(1, &modelMeshes[i].vao);
		glBindVertexArray(modelMeshes[i].vao);

		glGenBuffers(1, &modelMeshes[i].vbo);
		glBindBuffer(GL_ARRAY_BUFFER, modelMeshes[i].vbo);
		glBufferData(GL_ARRAY_BUFFER, modelMeshes[i].vertices.size() * sizeof(Vertex), &modelMeshes[i].vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); //0 = position of VA
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //configure it so that opengl knows how to read it

		glEnableVertexAttribArray(TEX_COORD_LOCATION);
		glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

		//create Normals Vertex Buffer Object and bind it to position 1
		glGenBuffers(1, &modelMeshes[i].NormalsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, modelMeshes[i].NormalsVBO);
		glBufferData(GL_ARRAY_BUFFER, modelMeshes[i].normals.size() * sizeof(glm::vec3), modelMeshes[i].normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(NORMAL_LOCATION); //1 = position of VA
		glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //configure it so that opengl knows how to read it

		//create Edge Buffer Object and bind it and store indices on it
		glGenBuffers(1, &modelMeshes[i].ibo); //1 object and pointing to refe	//glGenBuffers(1, &BonesVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelMeshes[i].ibo); //make object current object
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelMeshes[i].indices.size() * sizeof(unsigned int), modelMeshes[i].indices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &modelMeshes[i].bonesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, modelMeshes[i].bonesVBO);
		glBufferData(GL_ARRAY_BUFFER, modelMeshes[i].boneData.size() * sizeof(modelMeshes[i].boneData[0]), &modelMeshes[i].boneData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(BONE_ID_LOCATION);
		glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(BoneData), (void*)0);

		glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
		glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(BoneData), (const void*)(4 * sizeof(int32_t)));

		//unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}