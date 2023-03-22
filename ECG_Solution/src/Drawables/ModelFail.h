
#pragma once

#define _USE_MATH_DEFINES
#include "Drawable.h"
#include <cmath>
#include <scene.h>
#include <postprocess.h>
#include <Importer.hpp>
#include <vector>

class ModelFail : public Drawable {
private:
	Assimp::Importer importer; // https://assimp-docs.readthedocs.io/en/v5.1.0/ ... (An older Assimp website: http://assimp.sourceforge.net/lib_html/index.html)
	const aiScene* scene = nullptr;
	aiNode* root_node = nullptr; // Only being used in the: load_model_cout_console() function.
    
    struct Mesh
    {
        unsigned int VAO, VBO1, VBO2, VBO3, EBO;
        std::vector<glm::vec3> vert_positions;
        std::vector<glm::vec3> vert_normals;
        std::vector<glm::vec2> tex_coords;
        std::vector<unsigned int> vert_indices;
    };

public:
    int vertexCount;
    int indicesCount;

    unsigned int num_meshes;
    std::vector<Mesh> mesh_list;

	Model(const char* model_path) // Constructor
	{
		// http://assimp.sourceforge.net/lib_html/postprocess_8h.html (See: aiPostProcessSteps) (Flag options)

		// scene = importer.ReadFile(model_path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		scene = importer.ReadFile(model_path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);
		// scene = importer.ReadFile(model_path, aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
		// scene = importer.ReadFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);
		// scene = importer.ReadFile(model_path, NULL);			

		generateModel(); // Uncomment only one of these two load model functions.

		// load_model_cout_console();
	}

    void generateModel()
    {
        if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
            std::cout << "Assimp importer.ReadFile (Error) -- " << importer.GetErrorString() << "\n";
        else
        {
            num_meshes = scene->mNumMeshes;
            mesh_list.resize(num_meshes);

            aiMesh* mesh{};

            // (1) Loop through all the model's meshes
            // -----------------------------------------------------
            for (unsigned int i = 0; i < num_meshes; ++i)
            {
                mesh = scene->mMeshes[i]; // http://assimp.sourceforge.net/lib_html/structai_mesh.html				

                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; // http://assimp.sourceforge.net/lib_html/structai_material.html			

                // (3) Loop through all mesh [i]'s vertices
                // ---------------------------------------------------
                for (unsigned int i2 = 0; i2 < mesh->mNumVertices; ++i2)
                {
                    glm::vec3 position{};
                    position.x = mesh->mVertices[i2].x;
                    position.y = mesh->mVertices[i2].y;
                    position.z = mesh->mVertices[i2].z;
                    mesh_list[i].vert_positions.push_back(position);

                    if (mesh->HasNormals())
                    {
                        glm::vec3 normal{};
                        normal.x = mesh->mNormals[i2].x;
                        normal.y = mesh->mNormals[i2].y;
                        normal.z = mesh->mNormals[i2].z;
                        mesh_list[i].vert_normals.push_back(normal);
                    }
                    else
                        mesh_list[i].vert_normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

                    if (mesh->HasTextureCoords(0)) // Only slot [0] is in question.
                    {
                        glm::vec2 tex_coords{};
                        tex_coords.x = mesh->mTextureCoords[0][i2].x;
                        tex_coords.y = mesh->mTextureCoords[0][i2].y;
                        mesh_list[i].tex_coords.push_back(tex_coords);
                    }
                    else
                        mesh_list[i].tex_coords.push_back(glm::vec2(0.0f, 0.0f));
                }
                // (4) Loop through all mesh [i]'s Indices
                // --------------------------------------------------
                int indices_offset = 0;
                for (unsigned int i3 = 0; i3 < mesh->mNumFaces; ++i3)
                    for (unsigned int i4 = 0; i4 < mesh->mFaces[i3].mNumIndices; ++i4)
                        mesh_list[i].vert_indices.push_back(mesh->mFaces[i3].mIndices[i4] + indices_offset);

                // indices_offset += mesh->mNumVertices; // Disabled for tutorial: Model Loading (Part 1 of 3)

                set_buffer_data(i); // Set up: VAO, VBO and EBO.
            }
        }
    }

    void set_buffer_data(int index)      //unsigned int index
    {

        glGenVertexArrays(1, &mesh_list[index].VAO);
        glGenBuffers(1, &mesh_list[index].VBO1); // Alternative to using 3 separate VBOs, instead use only 1 VBO and set glVertexAttribPointer's offset...
        glGenBuffers(1, &mesh_list[index].VBO2); // like was done in tutorial 3... Orbiting spinning cubes.
        glGenBuffers(1, &mesh_list[index].VBO3);
        glGenBuffers(1, &mesh_list[index].EBO);

        glBindVertexArray(mesh_list[index].VAO);

        // Vertex Positions
        // ---------------------
        glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].vert_positions.size(), &mesh_list[index].vert_positions[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0); // Void pointer below is for legacy reasons. Two possible meanings: "offset for buffer objects" & "address for client state arrays"
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        // Vertex Normals
        // --------------------
        glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].vert_normals.size(), &mesh_list[index].vert_normals[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        // Indices for: glDrawElements()
        // ---------------------------------------
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_list[index].EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh_list[index].vert_indices.size(), &mesh_list[index].vert_indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0); 	// Unbind VAO
    };

    Model(std::string filePath);

};


