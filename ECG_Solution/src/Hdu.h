#pragma once

#include "Utils/Utils.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Drawables/Model.h"
#include "Utils/Transform.h"

struct HduData
{
	Shader simpleShader;
	Model model;
	Transform transform;
	float ndcXposition; // Position (normalized)
	float scaleMulti = 1.0f;
	glm::vec3 getScale(float widthScale, float heightScale) {
		return glm::vec3(widthScale * scaleMulti, heightScale * scaleMulti, 1.0f);
	}
};

class Hdu {
public:
	Hdu();

	Camera hduCamera;
	std::unordered_map<std::string, HduData> hduDataMap;
	const Transform emptyTransform;			// ugly but functional
	float widthScale, heightScale;		// Scale
	int screenWidth, screenHeight;
	int currentLifeNr;
	int maxLifeNr;

	void createHdu(GLFWwindow* window, Camera* threeDCamera, int lifeStartNumber);
	void drawHDU(GLFWwindow* window);
	void generateStartHDU();
	void initGameHDU(int lifeStartNumber);
	void calculateScale();
	void calculatePosition();
	void updateSpikeCount(int newNumber);
	void updateLifeCount(int newLifeNumber);
	void showBigScreen(string screenKey);
	float getFullscreenHDUScale();

};
