#pragma once
#include "GL/glew.h"
#include "stb_image.h"
#include <string>
#include <unordered_map>

class Texture {
public:
	Texture();
	Texture(const std::string& filepath, const std::string& textureName);
	void readTexture(const std::string& filepath, const std::string & textureName);
	void bind(const std::string& textureName);
	void unbind() const;
	~Texture();
private:
	std::unordered_map<std::string, unsigned int> ids;
};