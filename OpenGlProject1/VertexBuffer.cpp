#include "VertexBuffer.h"
#include "GL/glew.h"

VertexBuffer::VertexBuffer() = default;

VertexBuffer::VertexBuffer(const std::string& vbName, unsigned int size, const void* buffer){
	createVertexBuffer(vbName, size, buffer);
}

void VertexBuffer::createVertexBuffer(const std::string& vbName, unsigned int size, const void* buffer) {
	glGenBuffers(1, &ids[vbName]);
	glBindBuffer(GL_ARRAY_BUFFER, ids[vbName]);
	glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
}
void VertexBuffer::bind(const std::string& vbName){
	glBindBuffer(GL_ARRAY_BUFFER, ids[vbName]);
}

void VertexBuffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer() {
	for (auto &it : ids) {
		glDeleteBuffers(1, &(it.second));
	}	
	
}