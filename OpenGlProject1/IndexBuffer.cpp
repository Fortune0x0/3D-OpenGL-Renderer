#include "IndexBuffer.h"
#include "GL/glew.h"


IndexBuffer::IndexBuffer() = default;

IndexBuffer::IndexBuffer(const std::string& vbName, unsigned int size, const void* buffer) {
	createIndexBuffer(vbName, size, buffer);
}

void IndexBuffer::createIndexBuffer(const std::string& vbName, unsigned int size, const void * buffer) {
	glGenBuffers(1, &ids[vbName]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[vbName]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
}
void IndexBuffer::bind(const std::string& vbName) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[vbName]);
}

void IndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer() {
	for (auto& it : ids) {
		glDeleteBuffers(1, &(it.second));
	}

}