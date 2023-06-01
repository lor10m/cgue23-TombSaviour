#pragma once
#include "Utils.h"
#include "../Drawables/Model.h"

struct SpikeStruct {
	Model* model = NULL;
	glm::vec3 scale;
	glm::vec3 translate;
	glm::vec3 rotate;
	bool render;
};

struct CactusStruct {
	Model* model = NULL;
	glm::mat4 modelMatrix;
	//bool render;
};

class GlobalVariables {

public:
	int lightIdx;
	double lightArray[20];
	double illumination_multiplier;

};

void setIllumination(int newLightIdx);
int getIllumination();
double getIlluminationMultiplier();

