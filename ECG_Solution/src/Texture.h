#pragma once

#include "Utils.h"
#include "stb_image.h"

class Texture 
{

private:
	
public:
	Texture();
	Texture(const char* texturePath);
	void genTexture(const char* texturePath);
	void bind(int location);
	std::string type;
	std::string path;
	int width;
	int height;
	int nrChannels;
	GLuint handle;
};