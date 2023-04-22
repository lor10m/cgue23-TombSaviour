#include "Texture.h"
#include "Utils/stb_image.h"

//std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::textureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>();

Texture::Texture() {};

Texture::Texture(const char* texturePath)
{
	path = texturePath;
	genTexture(texturePath);
}

Texture::Texture(GLuint textureHandle) 
{
	handle = textureHandle;
}

void Texture::genTexture(const char* texturePath)
{
	path = texturePath;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0); // TODO: use DDSimage also here

	if (data)
	{
		GLenum format;
		if (nrChannels == 1) {
			format = GL_RED;
		}
		else if (nrChannels == 3) {
			format = GL_RGB;
		}
		else if (nrChannels == 4) {
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Loaded texture of size " << height << " x " << width << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind(int location)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, handle);
}


//GLuint Texture::getHDUTexHandlerFromPath2(const char* path) {
//
//	GLuint handle;
//	glGenTextures(1, &handle);
//	glBindTexture(GL_TEXTURE_2D, handle);
//
//	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0); // TODO: use DDSimage also here
//
//	if (data)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//	}
//	else {
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data);
//
//	glGenerateMipmap(GL_TEXTURE_2D);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
//
//	return handle;
//}

GLuint Texture::getHDUTexHandlerFromPath(const std::string path) {

	GLuint texHandler;
	glGenTextures(1, &texHandler); // generate buffer for texture on gpu
	glBindTexture(GL_TEXTURE_2D, texHandler);

	DDSImage img = loadDDS(path.c_str()); // load texture to cpu

	glCompressedTexImage2D(GL_TEXTURE_2D, 0, img.format, img.width, img.height, 0, img.size, img.data); // send data from cpu to gpu

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

	return texHandler;
}

std::shared_ptr<Texture> Texture::getHDUTexture(const std::string& texName) {
	/*
	if (textureMap.find("texName") == textureMap.end()) {	// not found
		string path = "assets/textures/" + texName + ".dds";
		GLuint handle = getHDUTexHandlerFromPath(path);
		textureMap.emplace(texName, handle);
	}
	else {	// found
		return textureMap[texName];
	}
	*/

	//string oneString = string(1, '1');	// texName
	string oneString = "hduTry";	// texName
	string path = "assets/textures/" + oneString + ".dds";
	GLuint handler = getHDUTexHandlerFromPath(path);
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(handler);
	//textureMap.emplace(texName, texture);
	//return textureMap[texName];
	return texture;
}


