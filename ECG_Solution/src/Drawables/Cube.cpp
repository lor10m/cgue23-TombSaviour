#include "Cube.h"
#include "unordered_map"
#include "../Vertex.h"

Cube::Cube(float width, float height, float depth) {
    generateCube(width, height, depth);
}

void Cube::generateCube(float width, float height, float depth) {
    int vertexCount = 8 * 3;
    int indicesCount = 12;
    float wh = width / 2;
    float hh = height / 2;
    float dh = depth / 2;

    Vertex vertices[24];

    vertices[0] = {{-wh, -hh, -dh}, {-1, 0, 0}, {0, 0}};
    vertices[1] = {{-wh, -hh, -dh}, {0, -1, 0}, {0, 0}};
    vertices[2] = {{-wh, -hh, -dh}, {0, 0, -1}, {1, 0}};
    vertices[3] = {{-wh, -hh, dh}, {-1, 0, 0}, {1, 0}};
    vertices[4] = {{-wh, -hh, dh}, {0, -1, 0}, {0, 1}};
    vertices[5] = {{-wh, -hh, dh}, {0, 0, 1}, {0, 0}};
    vertices[6] = {{-wh, hh, -dh}, {-1, 0, 0}, {0, 1}};
    vertices[7] = {{-wh, hh, -dh}, {0, 1, 0}, {0, 1}};
    vertices[8] = {{-wh, hh, -dh}, {0, 0, -1}, {1, 1}};
    vertices[9] = {{-wh, hh, dh}, {-1, 0, 0}, {1, 1}};
    vertices[10] = {{-wh, hh, dh}, {0, 1, 0}, {0, 0}};
    vertices[11] = {{-wh, hh, dh}, {0, 0, 1}, {0, 1}};
    vertices[12] = {{wh, -hh, -dh}, {1, 0, 0}, {1, 0}};
    vertices[13] = {{wh, -hh, -dh}, {0, -1, 0}, {1, 0}};
    vertices[14] = {{wh, -hh, -dh}, {0, 0, -1}, {0, 0}};
    vertices[15] = {{wh, -hh, dh}, {1, 0, 0}, {0, 0}};
    vertices[16] = {{wh, -hh, dh}, {0, -1, 0}, {1, 1}};
    vertices[17] = {{wh, -hh, dh}, {0, 0, 1}, {1, 0}};
    vertices[18] = {{wh, hh, -dh}, {1, 0, 0}, {1, 1}};
    vertices[19] = {{wh, hh, -dh}, {0, 1, 0}, {1, 1}};
    vertices[20] = {{wh, hh, -dh}, {0, 0, -1}, {0, 1}};
    vertices[21] = {{wh, hh, dh}, {1, 0, 0}, {0, 1}};
    vertices[22] = {{wh, hh, dh}, {0, 1, 0}, {1, 0}};
    vertices[23] = {{wh, hh, dh}, {0, 0, 1}, {1, 1}};

    GLuint indices[] = {
            0, 3, 9,
            9, 6, 0,
            5, 17, 23,
            23, 11, 5,
            15, 12, 18,
            18, 21, 15,
            14, 2, 8,
            8, 20, 14,
            10, 22, 19,
            19, 7, 10,
            1, 13, 16,
            16, 4, 1
    };

    iboSize = indicesCount * 3;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glDisableVertexAttribArray(0);
}
