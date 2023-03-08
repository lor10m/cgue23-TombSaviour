#include "Sphere.h"
#include "../Vertex.h"

Sphere::Sphere(int longSegments, int latSegments, float radius) {
    generateSphere(longSegments, latSegments, radius);
}

void Sphere::generateSphere(int longSegments, int latSegments, float radius) {
    int vertexCount = (2 + longSegments * (latSegments - 1));
    int indicesCount = (2 * longSegments * (latSegments - 1));
    double latAngleIncrease = M_PI / latSegments;
    double longAngleIncrease = M_PI * 2 / longSegments;

    Vertex* vertices = new Vertex[vertexCount];

    vertices[0] = {{0, radius, 0}, {0, 1, 0}, {0, 1}};
    vertices[vertexCount - 1] = {{0, -radius, 0}, {0, -1, 0}, {0, 0}};

    for (int i = 1; i < latSegments; ++i) {
        double latSin = sin(latAngleIncrease * i);
        double latCos = cos(latAngleIncrease * i);
        for (int j = 0; j < longSegments; ++j) {
            int latOffset = longSegments * (i - 1);
            vertices[1 + latOffset + j] = {{radius * latSin * cos(longAngleIncrease * j), radius * latCos, radius * latSin * sin(longAngleIncrease * j)}};
            vertices[1 + latOffset + j].normal = glm::normalize(vertices[1 + latOffset + j].position);
            // https://en.wikipedia.org/wiki/UV_mapping
            vertices[1 + latOffset + j].uv = {0.5 + ((atan2(vertices[1 + latOffset + j].position.z, vertices[1 + latOffset + j].position.x)) / (M_PI * 2)), 0.5 + glm::asin(vertices[1 + latOffset + j].position.y) / M_PI};
        }
    }

    GLuint* indices = new GLuint[indicesCount * 3];
    for (int j = 0; j < longSegments; ++j) {
        int longOffset = 3 * j;
        int vertAIndex = 1 + ((j + 1) % longSegments);
        int vertBIndex = j + 1;
        // Top cap
        indices[longOffset] = 0;
        indices[longOffset + 1] = vertAIndex;
        indices[longOffset + 2] = vertBIndex;
        // Bottom cap
        indices[indicesCount * 3 - (longOffset + 1)] = vertexCount - 1 - vertBIndex;
        indices[indicesCount * 3 - (longOffset + 2)] = vertexCount - 1 - vertAIndex;
        indices[indicesCount * 3 - (longOffset + 3)] = vertexCount - 1;
    }

    int topCapOffset = longSegments * 3;

    for (int i = 1; i < latSegments - 1; ++i) {
        int latOffset = longSegments * (i - 1) * 6;
        for (int j = 0; j < longSegments; ++j) {
            int longOffset = 6 * j;
            int vertAIndex = longSegments * (i - 1) + 1 + (j + 1) % longSegments;
            int vertBIndex = longSegments * (i - 1) + j + 1;
            int vertCIndex = longSegments * i + 1 + (j + 1) % longSegments;
            int vertDIndex = longSegments * i + j + 1;

            indices[topCapOffset + latOffset + longOffset] = vertBIndex;
            indices[topCapOffset + latOffset + longOffset + 1] = vertAIndex;
            indices[topCapOffset + latOffset + longOffset + 2] = vertCIndex;

            indices[topCapOffset + latOffset + longOffset + 3] = vertBIndex;
            indices[topCapOffset + latOffset + longOffset + 4] = vertCIndex;
            indices[topCapOffset + latOffset + longOffset + 5] = vertDIndex;
        }
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
