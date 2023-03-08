#pragma once

#define _USE_MATH_DEFINES
#include "Drawable.h"
#include <cmath>
class Torus: public Drawable {
private:
    void generateTorus(float R, float r, int segments, int crossSegments);
public:
    /**
     * @param R Distance from the center of the tube to the center of the torus
     * @param r Radius of the tube
     * @param segments Number of segments along the tube
     * @param crossSegments Number of segments across the tube
     */
    Torus(float R, float r, int segments, int crossSegments);
};
