#pragma once

#define _USE_MATH_DEFINES
#include "Drawable.h"
#include <cmath>

class Cube: public Drawable {
private:
    void generateCube(float width, float height, float depth);
public:
    Cube(float width, float height, float depth);
};