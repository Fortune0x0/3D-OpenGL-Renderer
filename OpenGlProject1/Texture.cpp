#include "Texture.h"
#include <iostream>
#include "GL/glew.h"

Texture::Texture() = default;

Texture::Texture(const std::string& filepath, const std::string& textureName) {
	readTexture(filepath, textureName);
}

void Texture::bind(const std::string& textureName)  {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ids[textureName]);
}

void Texture::readTexture(const std::string& filepath, const std::string& textureName) {
	stbi_set_flip_vertically_on_load(1);
	int width, height, originalChannels;
	
	unsigned char* buffer = stbi_load(filepath.c_str(), &width, &height, &originalChannels, 4);

	std::cout << "width: " << width << " height: " << height << " originalChennels: " << originalChannels << std::endl;

	glGenTextures(1, &ids[textureName]);
	glBindTexture(GL_TEXTURE_2D, ids[textureName]);
	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);


	glActiveTexture(GL_TEXTURE0);
	if (buffer) {
		stbi_image_free(buffer);
	}
	else {
		std::cout << "BUFFER NULL >>> " << std::endl;
	}



}
void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}
Texture::~Texture() {

	for (auto & it: ids) {
		glDeleteTextures(1, &it.second);
	}
	
}