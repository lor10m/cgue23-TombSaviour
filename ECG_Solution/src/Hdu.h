#pragma once

#include "Utils.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Drawables/Model.h"

class Hdu
{
private:


public:

	Hdu();
	Hdu(GLFWwindow* window, Camera threeDCamera);

	void initHdu();

	//std::shared_ptr<Camera> hduCamera;
	Camera hduCamera;

	Shader simpleShader;
	Model screenObject;

	void Hdu::drawHDU();

	std::shared_ptr<Texture> startTexture;

	void Hdu::renderFirstHDU(GLFWwindow* window, Camera threeDCamera);

	int width, height;

};
