#include "GlobalVariables.h"


int lightIdx = 10;
double lightArray[20] = { 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 };
double illumination_multiplier = 1.0;

int spikeCnt;
bool changeSpikeCnt;

void setIllumination(int newLightIdx) {
    lightIdx = newLightIdx;
}

int getIllumination() {
    return lightIdx;
}

double getIlluminationMultiplier() {
    return lightArray[lightIdx];
}
