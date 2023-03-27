#include "Model.h"
#include <postprocess.h>

Model::Model(string path)
{
	vertexCount = 0;
	iboSize = 0;
	generateModel(path);
}

void Model::generateModel(string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	std::cout << "Loading!";

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
	}
	else {
		processNode(scene->mRootNode, scene);
	}
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(const aiScene* scene) {

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D vertex = mesh->mVertices[i];
		vertices.push_back(glm::vec3(vertex.x, vertex.y, vertex.z));
		vertexCount++;

		if (!mesh->HasNormals()) {
			normals.push_back(glm::vec3(0, 1.0f, 0));
		}
		else {
			aiVector3D normal = mesh->mNormals[i];
			normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
		}

		if (mesh->mTextureCoords[0]) {
			aiVector3D uv = mesh->mTextureCoords[0][i];
			uvs.push_back(glm::vec2(uv.x, uv.y));
		}
		else {
			uvs.push_back(glm::vec2(0.0f, 0.0f));
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
			iboSize++;
		}
	}
	//TODO Material

	//TODO load in physx

	//create Vertex Array Object and bind it
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//create Vertex Buffer Object and bind it and store vertices on it
	glGenBuffers(1, &vbo); //1 object and pointing to reference
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //make object current object
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	//create Normals Vertex Buffer Object and bind it to position 1
	glGenBuffers(1, &NormalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, NormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	//create Edge Buffer Object and bind it and store indices on it
	glGenBuffers(1, &ibo); //1 object and pointing to reference
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //make object current object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //configure it so that opengl knows how to read it
	glEnableVertexAttribArray(0); //0 = position of VA

	glBindBuffer(GL_ARRAY_BUFFER, NormalsVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //configure it so that opengl knows how to read it
	glEnableVertexAttribArray(1); //1 = position of VA

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Model::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}