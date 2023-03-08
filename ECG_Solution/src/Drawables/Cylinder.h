#pragma once

#define _USE_MATH_DEFINES
#include "Drawable.h"
#include <cmath>

class Cylinder: public Drawable {
private:
    void generateCylinder(int segments, float height, float radius);
public:
    Cylinder(int segments, float height, float radius);
};