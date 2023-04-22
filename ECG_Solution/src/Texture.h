#pragma once

#include "Utils/Utils.h"
#include "Utils/stb_image.h"


class Texture 
{

private:
	
public:

	Texture();
	Texture(const char* texturePath);
	Texture(GLuint textureHandle);
	//Texture(const char* texturePath);
	void genTexture(const char* texturePath);
	void bind(int location);
	std::string type;
	std::string path;
	static GLuint getHDUTexHandlerFromPath(const char* path);
	std::shared_ptr<Texture> Texture::getHDUTexture(const std::string& texName);
	std::shared_ptr<Texture> loadTexture(const std::string& name);
	int width;
	int height;
	int nrChannels;

	//static std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;

	GLuint getHDUTexHandlerFromPath(const std::string path);

	GLuint handle;
};