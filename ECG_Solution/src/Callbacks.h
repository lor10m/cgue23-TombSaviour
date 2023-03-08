#pragma once

#include "Utils.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);