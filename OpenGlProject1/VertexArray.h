#pragma once
#include "GL/glew.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include <unordered_map>
#include <iostream>
class VertexArray {
public:
	VertexArray() = default;
	VertexArray(const std::string& vertexArrId){
		createVertexArray(vertexArrId);
	}

	void bind(const std::string & vertexArrId)  {
		glBindVertexArray(ids[vertexArrId]);
	}

	void createVertexArray(const std::string& vertexArrId) {
		glCreateVertexArrays(1, &ids[vertexArrId]);
		glBindVertexArray(ids[vertexArrId]);

	}
	void unbind() const {
		glBindVertexArray(0);
	}


	void addVertexLayout(const std::string vaName, const std::string & const vbName,  VertexBuffer &vb,  VertexBufferLayout & layout) {
		//bind(vaName);
		vb.bind(vbName);
		auto& elements = layout.getElements()[vbName];
		unsigned int offset = 0;

		for (std::size_t i = 0; i < elements.size(); ++i) {
			const auto& element = elements[i];
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(vbName), (const void*)offset);
			glEnableVertexAttribArray(i);
				offset += element.count * BufferElements::getTypeSize(element.type);

		}


}

	~VertexArray() {
		for (auto& id : ids) {
			glDeleteVertexArrays(1, &(id.second));
		}
	}
private:
	std::unordered_map<std::string, unsigned int> ids;

};