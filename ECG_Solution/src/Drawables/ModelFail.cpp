//
//#include "Model.h"
//#include <GL/glew.h>
//#include <string>
//#include <Importer.hpp>
//#include <postprocess.h>
//#include <glslang/Include/Common.h>
//#include <scene.h>
//
//
//Model::Model(std::string filePath) {
//    Assimp::Importer importer;
//    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate);
//
//    mVertices.reserve(scene->mNumMeshes * 3); // * 3 ?
//
//    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
//        const aiMesh* mesh = scene->mMeshes[i];
//        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
//            const aiVector3D& pos = mesh->mVertices[j];
//            mVertices.push_back(pos.x);
//            mVertices.push_back(pos.y);
//            mVertices.push_back(pos.z);
//        }
//    }
//
//    glGenBuffers(1, &mVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
//    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW);
//
//    mNumVertices = mVertices.size() / 3;
//}
//
//void Model::render() const {
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
//    glDrawArrays(GL_TRIANGLES, 0, mNumVertices);
//    glDisableVertexAttribArray(0);
//}
//
