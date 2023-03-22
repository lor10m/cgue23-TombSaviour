#pragma once

#define _USE_MATH_DEFINES
#include "Drawable.h"
#include <cmath>
#include <Importer.hpp>
#include <mesh.h>
#include <vector>


class Model : public Drawable {
private:

    Assimp::Importer importer; // https://assimp-docs.readthedocs.io/en/v5.1.0/ ... (An older Assimp website: http://assimp.sourceforge.net/lib_html/index.html)
    const aiScene* scene = nullptr;
    aiNode* root_node = nullptr; // Only being used in the: load_model_cout_console() function.

    void generateModel(string path);
    void generateModel1(float width, float height, float depth);

    struct Mesh
    {
        unsigned int VAO, VBO1, VBO2, VBO3, EBO;
        std::vector<glm::vec3> vert_positions;
        std::vector<glm::vec3> vert_normals;
        std::vector<glm::vec2> tex_coords;
        std::vector<unsigned int> vert_indices;
    };

public:
    unsigned int num_meshes;
    std::vector<Mesh> mesh_list;

    Model(float width, float height, float depth);
    Model(string path);

    void set_buffer_data(int index)      //unsigned int index
    {

        glGenVertexArrays(1, &mesh_list[index].VAO);
        glBindVertexArray(mesh_list[index].VAO);

        glGenBuffers(1, &mesh_list[index].VBO1); // Alternative to using 3 separate VBOs, instead use only 1 VBO and set glVertexAttribPointer's offset...
        glGenBuffers(1, &mesh_list[index].VBO2); // like was done in tutorial 3... Orbiting spinning cubes.
        glGenBuffers(1, &mesh_list[index].VBO3);
        glGenBuffers(1, &mesh_list[index].EBO);


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

        // Texture is missing here:
        //glEnableVertexAttribArray(2);
        //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

        // Indices for: glDrawElements()
        // ---------------------------------------
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_list[index].EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh_list[index].vert_indices.size(), &mesh_list[index].vert_indices[0], GL_STATIC_DRAW);

        //glBindVertexArray(0); 	// Unbind VAO
        //glDisableVertexAttribArray(0);
    };

    void draw2() {
        glBindVertexArray(mesh_list[0].VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei)mesh_list[0].vert_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

};