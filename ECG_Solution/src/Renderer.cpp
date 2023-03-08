#include "Renderer.h"

void Renderer::drawTeapot(Shader& shader) const {
    shader.activate();
    ::drawTeapot();
}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderDrawable(Shader &shader, Drawable& drawable) {
    shader.activate();
    drawable.draw();
}
