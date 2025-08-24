#pragma once
#include <unordered_map>
#include <string>
class VertexBuffer {
public:
	VertexBuffer();
	VertexBuffer(const std::string & vbName, unsigned int size, const void* buffer);
	void createVertexBuffer(const std::string& vbName, unsigned int size, const void* buffer);
	void bind(const std::string& vbName);
	void unbind() const;
	~VertexBuffer();
private:
	std::unordered_map<std::string, unsigned int> ids;
};