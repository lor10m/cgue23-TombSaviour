#pragma once
#include "Utils.h"

class GlobalVariables {

public:
	int lightIdx;
	double lightArray[20];
	double illumination_multiplier;
};

void setIllumination(int newLightIdx);
int getIllumination();
