#pragma once

#define _USE_MATH_DEFINES
#include "Drawable.h"
#include <cmath>
#include <Importer.hpp>
#include <mesh.h>
#include <vector>
#include <scene.h>


class Model : public Drawable {
private:

    Assimp::Importer importer; // https://assimp-docs.readthedocs.io/en/v5.1.0/ ... (An older Assimp website: http://assimp.sourceforge.net/lib_html/index.html)
    const aiScene* scene = nullptr;
    aiNode* root_node = nullptr; // Only being used in the: load_model_cout_console() function.

    void generateModel(string path);
    void generateModel1(float width, float height, float depth);

    struct sVertex
    {
        float x, y, z;
        float nx, ny, nz;
        float u0, v0;
    };

    struct sTriangle
    {
        unsigned int vertIndex[3];
    };

public:
    unsigned int num_meshes;

    sVertex vertex;
    std::vector<sVertex> vertices;
    sTriangle face;
    std::vector<sTriangle> faces;
    unsigned int VAO, VBO, EBO;

    Model(float width, float height, float depth);
    Model(string path);


    void SetupMesh()      //unsigned int index
    {
        // Generate IDs for our VAO, VBO and EBO
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);

        // Tell open GL where to look for for vertex data
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(sVertex), &vertices[0], GL_STATIC_DRAW);

        // Tell open GL where our index buffer begins (AKA: where to look for faces)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(sTriangle), &faces[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);       // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)offsetof(sVertex, x));

        glEnableVertexAttribArray(1);       // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)offsetof(sVertex, nx));

        //glEnableVertexAttribArray(2);       // textureCoordinates
        //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)offsetof(sVertex, u0));

        // Now that all the parts are set up, unbind buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

    };

    void draw() {
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->faces.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};