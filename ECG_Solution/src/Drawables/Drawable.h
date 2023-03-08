#pragma once

#include "../Utils.h"

class Drawable {
protected:
    GLuint vao{};
    GLuint vbo{};
    GLuint ibo{};
    int iboSize{};
public:
    ~Drawable() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }
    void draw() const {
        glEnableVertexAttribArray(0);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, iboSize, GL_UNSIGNED_INT, nullptr);
        glDisableVertexAttribArray(0);
    }
};
