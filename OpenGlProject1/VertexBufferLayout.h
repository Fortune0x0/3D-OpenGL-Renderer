#pragma once
#include "GL/glew.h"
#include <unordered_map>
#include <string>
#include <vector>
struct BufferElements {
	unsigned int count, normalized, type; 


	static unsigned int getTypeSize(unsigned int type) {
		switch (type) {
		case GL_FLOAT: return sizeof(float);
		case GL_UNSIGNED_INT: return sizeof(unsigned int);
		case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
		}

		return 0;
	}
};


class VertexBufferLayout {
public:
	VertexBufferLayout()  {

	}
	
	template<typename> void push(const std::string vbName, unsigned int count);

	template<>
	void push<float>(const std::string vbName, unsigned int count) {
		strides[vbName] += count * BufferElements::getTypeSize(GL_FLOAT);
		elements[vbName].push_back({count, GL_FALSE, GL_FLOAT});
	}

	template<>
	void push<unsigned int>(const std::string vbName, unsigned int count) {
		strides[vbName] += count * BufferElements::getTypeSize(GL_UNSIGNED_INT);
		elements[vbName].push_back({count, GL_FALSE, GL_UNSIGNED_INT});
	}

	template<>
	void push<unsigned char>(const std::string vbName, unsigned int count) {
		strides[vbName] += count * BufferElements::getTypeSize(GL_UNSIGNED_BYTE);
		elements[vbName].push_back({count, GL_FALSE, GL_UNSIGNED_BYTE});
	}

	 auto& getElements() { return elements;}
	const unsigned int& getStride(const std::string & vbName)  { return strides[vbName]; }
private:
	std::unordered_map<std::string, unsigned int> strides;
	std::unordered_map<std::string, std::vector<BufferElements>> elements;
};