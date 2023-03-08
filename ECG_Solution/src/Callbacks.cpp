#include "Callbacks.h"

static GLenum currentPolygonMode = GL_FILL;
static GLboolean isBackfaceCullingActive = GL_TRUE;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        if (currentPolygonMode == GL_FILL) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            currentPolygonMode = GL_LINE;
        } else if (currentPolygonMode == GL_LINE) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            currentPolygonMode = GL_FILL;
        }
    }
    if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
        if (isBackfaceCullingActive == GL_TRUE) {
            glDisable(GL_CULL_FACE);
            isBackfaceCullingActive = GL_FALSE;
        } else {
            glEnable(GL_CULL_FACE);
            isBackfaceCullingActive = GL_TRUE;
        }
    }
}

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    // https://www.khronos.org/opengl/wiki/Debug_Output
    if (id == 131185 || id == 131218) return; // ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia

    std::string sourceText;
    switch (source) {
        case GL_DEBUG_SOURCE_API:             sourceText = "API"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     sourceText = "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           sourceText = "Other"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceText = "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceText = "Third Party"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceText = "Window System"; break;
    }

    std::string typeText;
    switch (type) {
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeText = "Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_ERROR:               typeText = "Error"; break;
        case GL_DEBUG_TYPE_MARKER:              typeText = "Marker"; break;
        case GL_DEBUG_TYPE_OTHER:               typeText = "Other"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         typeText = "Performance"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           typeText = "Pop Group"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         typeText = "Portability"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          typeText = "Push Group"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeText = "Undefined Behaviour"; break;
    }

    std::string severityText;
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         severityText = "High"; break;
        case GL_DEBUG_SEVERITY_LOW:          severityText = "Low"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       severityText = "Medium"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: severityText = "Notification"; break;
    }

    fprintf(stderr,
            "%s [ID: %u, Source: %s, Type: %s, Severity: %s]\n",
            message, id, sourceText.c_str(), typeText.c_str(), severityText.c_str()
    );
}