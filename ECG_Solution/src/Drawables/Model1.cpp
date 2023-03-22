//
//#include "Model.h"
//#include "unordered_map"
//#include "../Vertex.h"
//#include <postprocess.h>
//
//#include <glslang/Include/Common.h>
//#include <scene.h>
//
//// simple example: https://subscription.packtpub.com/book/game-development/9781838986193/2/ch02lvl1sec18/using-the-assimp-library
//
//Model::Model(float width, float height, float depth) {
//    generateModel1(width, height, depth);
//}
//
//Model::Model(string path)
//{
//    generateModel(path);
//}
//
//void Model::generateModel1(float width, float height, float depth) {
//    int vertexCount = 8 * 3;
//    int indicesCount = 12;
//    float wh = width / 2;
//    float hh = height / 2;
//    float dh = depth / 2;
//
//    Vertex vertices[24];
//
//    vertices[0] = { {-wh, -hh, -dh}, {-1, 0, 0}, {0, 0} };
//    vertices[1] = { {-wh, -hh, -dh}, {0, -1, 0}, {0, 0} };
//    vertices[2] = { {-wh, -hh, -dh}, {0, 0, -1}, {1, 0} };
//    vertices[3] = { {-wh, -hh, dh}, {-1, 0, 0}, {1, 0} };
//    vertices[4] = { {-wh, -hh, dh}, {0, -1, 0}, {0, 1} };
//    vertices[5] = { {-wh, -hh, dh}, {0, 0, 1}, {0, 0} };
//    vertices[6] = { {-wh, hh, -dh}, {-1, 0, 0}, {0, 1} };
//    vertices[7] = { {-wh, hh, -dh}, {0, 1, 0}, {0, 1} };
//    vertices[8] = { {-wh, hh, -dh}, {0, 0, -1}, {1, 1} };
//    vertices[9] = { {-wh, hh, dh}, {-1, 0, 0}, {1, 1} };
//    vertices[10] = { {-wh, hh, dh}, {0, 1, 0}, {0, 0} };
//    vertices[11] = { {-wh, hh, dh}, {0, 0, 1}, {0, 1} };
//    vertices[12] = { {wh, -hh, -dh}, {1, 0, 0}, {1, 0} };
//    vertices[13] = { {wh, -hh, -dh}, {0, -1, 0}, {1, 0} };
//    vertices[14] = { {wh, -hh, -dh}, {0, 0, -1}, {0, 0} };
//    vertices[15] = { {wh, -hh, dh}, {1, 0, 0}, {0, 0} };
//    vertices[16] = { {wh, -hh, dh}, {0, -1, 0}, {1, 1} };
//    vertices[17] = { {wh, -hh, dh}, {0, 0, 1}, {1, 0} };
//    vertices[18] = { {wh, hh, -dh}, {1, 0, 0}, {1, 1} };
//    vertices[19] = { {wh, hh, -dh}, {0, 1, 0}, {1, 1} };
//    vertices[20] = { {wh, hh, -dh}, {0, 0, -1}, {0, 1} };
//    vertices[21] = { {wh, hh, dh}, {1, 0, 0}, {0, 1} };
//    vertices[22] = { {wh, hh, dh}, {0, 1, 0}, {1, 0} };
//    vertices[23] = { {wh, hh, dh}, {0, 0, 1}, {1, 1} };
//
//    GLuint indices[] = {
//            0, 3, 9,
//            9, 6, 0,
//            5, 17, 23,
//            23, 11, 5,
//            15, 12, 18,
//            18, 21, 15,
//            14, 2, 8,
//            8, 20, 14,
//            10, 22, 19,
//            19, 7, 10,
//            1, 13, 16,
//            16, 4, 1
//    };
//
//    iboSize = indicesCount * 3;
//
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//
//    glGenBuffers(1, &vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
//
//    glGenBuffers(1, &ibo);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);
//
//    glDisableVertexAttribArray(0);
//}
//
//void Model::generateModel(string filePath) {
//    std::cout << filePath;
//
//    Assimp::Importer importer;
//    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate);
//
//    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
//        std::cout << "Assimp importer.ReadFile (Error) -- " << importer.GetErrorString() << "\n";
//    else
//    {
//        num_meshes = scene->mNumMeshes;
//        // std::cout << "Number Meshes: " << num_meshes;
//        mesh_list.resize(num_meshes);
//
//        aiMesh* mesh{};
//        //int vertexCount = scene->mNumMeshes * 3;
//        int numTriangles = scene->mMeshes[0]->mNumFaces;
//        int indicesCount = numTriangles * 3;
//        iboSize = numTriangles * 3;
//
//        // (1) Loop through all the model's meshes
//            // -----------------------------------------------------
//        for (unsigned int i = 0; i < num_meshes; ++i)
//        {
//            mesh = scene->mMeshes[i]; // http://assimp.sourceforge.net/lib_html/structai_mesh.html				
//
//            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; // http://assimp.sourceforge.net/lib_html/structai_material.html			
//
//            std::cout << "Number vertices: " << mesh->mNumVertices;
//
//            // (3) Loop through all mesh [i]'s vertices
//            // ---------------------------------------------------
//            for (unsigned int i2 = 0; i2 < mesh->mNumVertices; ++i2)
//            {
//                glm::vec3 position{};
//                position.x = mesh->mVertices[i2].x;
//                position.y = mesh->mVertices[i2].y;
//                position.z = mesh->mVertices[i2].z;
//                mesh_list[i].vert_positions.push_back(position);
//
//                std::cout << "\nMesh Position x: " << mesh_list[i].vert_positions[i2].x;
//                std::cout << "y: " << mesh_list[i].vert_positions[i2].y;
//                std::cout << "z: " << mesh_list[i].vert_positions[i2].z << "\n";
//                
//                if (mesh->HasNormals())
//                {
//                    glm::vec3 normal{};
//                    normal.x = mesh->mNormals[i2].x;
//                    normal.y = mesh->mNormals[i2].y;
//                    normal.z = mesh->mNormals[i2].z;
//                    mesh_list[i].vert_normals.push_back(normal);
//                }
//                else
//                    mesh_list[i].vert_normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
//            }
//
//            int indices_offset = 0;
//            for (unsigned int i3 = 0; i3 < mesh->mNumFaces; ++i3)
//                for (unsigned int i4 = 0; i4 < mesh->mFaces[i3].mNumIndices; ++i4)
//                    mesh_list[i].vert_indices.push_back(mesh->mFaces[i3].mIndices[i4] + indices_offset);
//            //std::cout << mesh->mNumFaces;
//
//            //set_buffer_data(i);
//        }
//    }
//}
//
//
