#pragma once
#include <string>
#include <unordered_map>
class IndexBuffer {
public:
	IndexBuffer();
	IndexBuffer(const std::string& vbName, unsigned int size, const void * buffer);
	void createIndexBuffer(const std::string& vbName, unsigned int size, const void* buffer);
	void bind(const std::string& vbName);
	void unbind() const;
	~IndexBuffer();
private:
	std::unordered_map<std::string, unsigned int> ids;
};