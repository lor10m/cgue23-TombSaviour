#pragma once

#include "Shader.h"
#include "Drawables/Drawable.h"

class Renderer {
public:
    void clear() const;
    void renderDrawable(Shader& shader, Drawable& drawable);
    void drawTeapot(Shader& shader) const;
};