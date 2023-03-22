#pragma once

#include "Utils.h"
#include "stb_image.h"

class Texture 
{

private:
	GLuint handle;

public:
	Texture();
	Texture(const char* texturePath);
	void genTexture(const char* texturePath);
	void bind(int location);
	int width;
	int height;
	int nrChannels;
};