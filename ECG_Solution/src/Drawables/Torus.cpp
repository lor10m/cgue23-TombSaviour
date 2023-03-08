#include "Torus.h"

Torus::Torus(float R, float r, int segments, int crossSegments) {
    generateTorus(R, r, segments, crossSegments);
}

void Torus::generateTorus(float R, float r, int segments, int crossSegments) {
    int vertexCount = segments * crossSegments;
    int indicesCount = 2 * vertexCount;
    double angleIncrease = M_PI * 2 / segments;
    double crossAngleIncrease = M_PI * 2 / crossSegments;

    GLfloat* verticesPositions = new GLfloat[vertexCount * 3];

    for (int i = 0; i < segments; ++i) {
        double majorSin = sin(angleIncrease * i);
        double majorCos = cos(angleIncrease * i);
        for (int j = 0; j < crossSegments; ++j) {
            int segmentOffset = crossSegments * i * 3;
            int crossOffset = 3 * j;
            verticesPositions[segmentOffset + crossOffset] = (float)((R + r * cos(crossAngleIncrease * j)) * majorCos);
            verticesPositions[segmentOffset + crossOffset + 1] = (float)((R + r * cos(crossAngleIncrease * j)) * majorSin);
            verticesPositions[segmentOffset + crossOffset + 2] = (float)(r * sin(crossAngleIncrease * j));
        }
    }

    GLuint* indices = new GLuint[indicesCount * 3];

    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < crossSegments; ++j) {
            int segmentOffset = crossSegments * i * 6;
            int crossOffset = 6 * j;

            int vertAIndex = crossSegments * i + j;
            int vertBIndex = crossSegments * i + (j + 1) % crossSegments;
            int vertCIndex = (crossSegments * (i + 1)) % vertexCount + j;
            int vertDIndex = (crossSegments * (i + 1)) % vertexCount + (j + 1) % crossSegments;

            indices[segmentOffset + crossOffset] = vertBIndex;
            indices[segmentOffset + crossOffset + 1] = vertAIndex;
            indices[segmentOffset + crossOffset + 2] = vertCIndex;

            indices[segmentOffset + crossOffset + 3] = vertBIndex;
            indices[segmentOffset + crossOffset + 4] = vertCIndex;
            indices[segmentOffset + crossOffset + 5] = vertDIndex;
        }
    }

    iboSize = indicesCount * 3;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), verticesPositions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glDisableVertexAttribArray(0);

    delete[] verticesPositions;
    delete[] indices;
}
