#pragma once

#define _USE_MATH_DEFINES
#include "Drawable.h"
#include <cmath>

class Cube: public Drawable {
public:
    Cube();
    void generateCube(float width, float height, float depth);
};