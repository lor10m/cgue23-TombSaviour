#include "Cylinder.h"
#include "../Vertex.h"

Cylinder::Cylinder(int segments, float height, float radius) {
    generateCylinder(segments, height, radius);
}

void Cylinder::generateCylinder(int segments, float height, float radius) {
    int indicesCount = segments * 4;
    int vertexCount = 2 * (2 * segments + 2) - 2;
    Vertex* vertices = new Vertex[vertexCount];
    double angleIncrease = M_PI * 2 / segments;
    /** segment offset for index array, i.e. where the lower circle positions begin, excluding center*/
    int sIOffset = segments + 1;
    int fOffset = (2 * segments + 2) - 1;

    vertices[0] = {{0, height / 2, 0}, {0, 1, 0}, {0.5f, 0.5f}};
    vertices[sIOffset] = {{0, -height / 2, 0}, {0, -1, 0}, {0.5f, 0.5f}};

    for (int i = 1; i <= segments; ++i) {
        // Top circle
        vertices[i] = {{radius * sin(angleIncrease * i), height / 2, radius * cos(angleIncrease * i)}, {0, 1, 0}};
        vertices[i].uv = {(vertices[i].position.x + radius) / (2 * radius), (vertices[i].position.z + radius) / (2 * radius)};
        vertices[fOffset + i] = {vertices[i].position, glm::normalize(glm::vec3(vertices[i].position.x, 0, vertices[i].position.z)), {(double) i / segments, 1}};
        // Bottom circle
        vertices[sIOffset + i] = {{vertices[i].position.x, -height / 2, vertices[i].position.z}, {0, -1, 0}};
        vertices[sIOffset + i].uv = {(vertices[sIOffset + i].position.x + radius) / (2 * radius), (-vertices[sIOffset + i].position.z + radius) / (2 * radius)};
        vertices[fOffset - 1 + sIOffset + i] = {vertices[sIOffset + i].position, vertices[fOffset + i].normal, {(double) i / segments, 0}};
    }

    GLuint* indices = new GLuint[indicesCount * 3];
    for (int i = 1, j = 0; i <= segments; ++i, ++j) {
        // Top triangle
        indices[12 * j] = i;
        indices[12 * j + 1] = 1 + (i % segments);
        indices[12 * j + 2] = 0;
        // Side quad
        indices[12 * j + 3] = fOffset + 1 + (i % segments);
        indices[12 * j + 4] = fOffset + i;
        indices[12 * j + 5] = fOffset - 1 + sIOffset + i;

        indices[12 * j + 6] = fOffset - 1 + sIOffset + i;
        indices[12 * j + 7] = fOffset - 1 + sIOffset + 1 + (i % segments);
        indices[12 * j + 8] = fOffset + 1 + (i % segments);
        // Bottom triangle
        indices[12 * j + 9] = sIOffset;
        indices[12 * j + 10] = sIOffset + 1 + (i % segments);
        indices[12 * j + 11] = sIOffset + i;
    }

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

    delete[] vertices;
    delete[] indices;
}
