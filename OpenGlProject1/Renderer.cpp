#define _USE_MATH_DEFINES
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <unordered_map>
Renderer::Renderer() {
	createCubeIndexBuffer();
	createTexturedCubeIndexBuffer();

	vertices["cubeModelMatrix"].resize(16);
	vertices["cubeViewMatrix"].resize(16);
	vertices["cubeProjectionMatrix"].resize(16);

	vertices["xRotation"].resize(16);
	vertices["yRotation"].resize(16);
	vertices["zRotation"].resize(16);



	initMatrices();
}


template<typename T>
void Renderer::addCounts(const std::string & vbName, unsigned int count) {
	vertexLayout.push<T>(vbName, count);

}

template<typename T, typename... Args>
void Renderer::createVertexLayout(const std::string vaName, const std::string& vbName, Args... counts) {
	(addCounts<T>(vbName, counts), ...);
	vertexArr.addVertexLayout(vaName, vbName, vertexBuff, vertexLayout);
}
void Renderer::assignCircleVertices(const std::string& circleName, const float& centerX, const float& centerY, const float& radius, const int& res,
	float r, float g, float b, float a
	
	) {

	int bufferSize = (res + 2) + (2 * (res + 2));
	float* circleBuff;
	vertices[circleName].resize(bufferSize);
	circleBuff = vertices[circleName].data();


	circleBuff[0] = centerX;
	circleBuff[1] = centerY;
	circleBuff[2] = r, circleBuff[3] = g, circleBuff[4] = b, circleBuff[5] = a;

	int vertices = (res/2) - 1; //-1 to account for vertices (centerX, centerY)
	for (int i = 0; i <= vertices; ++i) {
		float angle = 2 * M_PI / vertices * i;

		circleBuff[6 * i + 6] = centerX + std::cos(angle) * radius;
		circleBuff[6 * i + 6 + 1] = centerY + std::sin(angle) * radius;


		circleBuff[6 * i + 6 + 2] = r;
		circleBuff[6 * i + 6 + 3] = g;
		circleBuff[6 * i + 6 + 4] = b;
		circleBuff[6 * i + 6 + 5] = a;

	}


	vertexArr.createVertexArray(circleName);
	vertexBuff.createVertexBuffer(circleName, bufferSize * sizeof(float), circleBuff);
	createVertexLayout<float>(circleName, circleName, 2, 4);
	
}

void Renderer::assignTriangleVertices(const std::string & triangleName, float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float a) {
	float* buffer;
	vertices[triangleName].resize(18);
	buffer = vertices[triangleName].data();


	buffer[0] = x1;
	buffer[1] = y1;
	buffer[2] = r, buffer[3] = g, buffer[4] = b, buffer[5] = a;

	buffer[6] = x2;
	buffer[7] = y2;
	buffer[8] = r, buffer[9] = g, buffer[10] = b, buffer[11] = a;

	buffer[12] = x3;
	buffer[13] = y3;
	buffer[14] = r, buffer[15] = g, buffer[16] = b, buffer[17] = a;

	vertexArr.createVertexArray(triangleName);

	vertexBuff.createVertexBuffer(triangleName, 18 * sizeof(float), buffer);

	createVertexLayout<float>(triangleName, triangleName, 2, 4);

}
void Renderer::rotateShape(const std::string& shape, float radius, float offset) {
	if (shape.find("Circle") != std::string::npos) {
		if (vertices[shape].size() == 0) {
			assignCircleVertices(shape);
		}
	}
	if (shape.find("Triangle") != std::string::npos) {
		if (vertices[shape].size() == 0) {
			assignTriangleVertices(shape);
		}
		
	}

	rotate(shape, radius, offset);
}
void Renderer::rotate(const std::string& shape, float radius, float offset) {
	shader.bind("RotationShader");
	shader.setUniform1f("RotationShader", "angle", glfwGetTime());
	shader.setUniform1f("RotationShader", "radius", radius);
	shader.setUniform1f("RotationShader", "offset", offset);


	if (shape.find("Circle") != std::string::npos) {
		
		vertexArr.bind(shape);
		vertexBuff.bind(shape);
		glDrawArrays(GL_TRIANGLE_FAN, 0, (vertices[shape].size() / 3) / 2);
	}
	if (shape.find("Triangle") != std::string::npos) {
		vertexArr.bind(shape);
		vertexBuff.bind(shape);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
void Renderer::drawTriangle(const std::string& triangleName, float x1, float y1, float x2, float y2 , float x3 , float y3,
	float r , float g , float b , float a) {
	
	if (vertices[triangleName].size() == 0) {
		assignTriangleVertices(triangleName, x1, y1, x2, y2, x3, y3, r, g, b, a);
	}
	
	vertexArr.bind(triangleName);
	vertexBuff.bind(triangleName);
	shader.bind("BasicShader");
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::drawCircle(const std::string& circleName, float centerX, float centerY, float radius, int res, float f1, float f2 , float f3 , float f4) {
	if (vertices[circleName].size()  == 0 ) {
		assignCircleVertices(circleName, centerX, centerY, radius, res,  f1, f2, f3, f4);
		
	}
	vertexArr.bind(circleName);
	vertexBuff.bind(circleName);
	shader.bind("BasicShader");
	
	
	unsigned int drawArrayCount  = (vertices[circleName].size() / 3)/2;
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, drawArrayCount);
}



void Renderer::assignTriangleTex(const std::string& triangleName) {
	vertices[triangleName].resize(16);
	float* buffer = vertices[triangleName].data();

	buffer[4] = 0.0f, buffer[5] = 0.8f;
	buffer[6] = 0.5f, buffer[7] = 1.0f;

	buffer[8] = -0.8f, buffer[9] = 0.0f;
	buffer[10] = 0.0f, buffer[11] = 0.0f;

	buffer[0] = 0.8f, buffer[1] = 0.0f;
	buffer[2] = 1.0f, buffer[3] = 0.0f;


	vertexArr.createVertexArray(triangleName);

	vertexBuff.createVertexBuffer(triangleName, 16 * sizeof(float), buffer);

	createVertexLayout<float>(triangleName, triangleName, 2, 2);

}


void Renderer::drawTriangleTex(const std::string& texpath) {
	if (vertices["TriangleTex"].size() == 0) {
		assignTriangleTex("TriangleTex");
		tex.readTexture(texpath, "triangleTex");
		shader.bind("TextureShader");
	}
	
	
	
	vertexArr.bind("TriangleTex");
	vertexBuff.bind("TriangleTex");
	shader.bind("TextureShader");
	tex.bind("triangleTex");
	//shader.setUniformi("TextureShader", "texUnit", 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);


}
void Renderer::assignCubeVertices(const std::string& cubeName, float x, float y, float z, float r, float g, float b, float a) {
	vertices[cubeName].resize(56);
	float *buffer = vertices[cubeName].data();



	//cube top
	buffer[0] = x,     buffer[1] = y,      buffer[2] = z;
	buffer[3] = r,     buffer[4] = g,      buffer[5] = b, buffer[6] = a;

	buffer[7] = -x,    buffer[8]  = y,     buffer[9] = z;
	buffer[10] = r,    buffer[11] = g,     buffer[12] = b, buffer[13] = a;

	buffer[14] = -x,   buffer[15]  = -y,   buffer[16] = z;
	buffer[17] = r,    buffer[18] = g,     buffer[19] = b, buffer[20] = a;

	buffer[21] =  x,   buffer[22] = -y,    buffer[23] = z;
	buffer[24] = r,    buffer[25] = g,     buffer[26] = b, buffer[27] = a;

	
	//cube bottom
	buffer[28] = x,     buffer[29] = y,      buffer[30] = -z;
	buffer[31] = r,     buffer[32] = g,      buffer[33] = b, buffer[34] = a;

	buffer[35] = -x,    buffer[36] = y,      buffer[37] = -z;
	buffer[38] = r,     buffer[39] = g,      buffer[40] = b, buffer[41] = a;

	buffer[42] = -x,    buffer[43] = -y,     buffer[44] = -z;
	buffer[45] = r,     buffer[46] = g,      buffer[47] = b, buffer[48] = a;

	buffer[49] = x,     buffer[50] = -y,     buffer[51] = -z;
	buffer[52] = r,     buffer[53] = g,      buffer[54] = b, buffer[55] = a;


	vertexArr.createVertexArray(cubeName);
	vertexBuff.createVertexBuffer(cubeName, 56 * sizeof(float), buffer);
	createVertexLayout<float>(cubeName, cubeName, 3, 4);

}


void Renderer::assignSceneCubeTexCoord(const std::string & sceneName) {
	vertices[sceneName].resize(120);
	float* buffer = vertices[sceneName].data();
	
	int index = 0;
	//front face																	
	//top right																					//Texture coordinates
	buffer[index++] = 0.5f;	 buffer[index++] = 0.5f;      buffer[index++] = -0.5f;		buffer[index++] = 1.0f;  buffer[index++] = 1.0f;
	//top left
	buffer[index++] = -0.5f; buffer[index++] = 0.5f;      buffer[index++] = -0.5f;		buffer[index++] = 0.0f;  buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = 0.5f;  buffer[index++] = -0.5f;     buffer[index++] = -0.5f;		buffer[index++] = 1.0f;  buffer[index++] = 0.0f;
	//bottom left
	buffer[index++] = -0.5f; buffer[index++] = -0.5f;     buffer[index++] = -0.5f;		buffer[index++] = 0.0f;  buffer[index++] = 0.0f;
	

	//back face
	//top right
	buffer[index++] = 0.5f;  buffer[index++] = 0.5f;	 buffer[index++] = 0.5f;		buffer[index++] = 1.0f;  buffer[index++] = 1.0f;
	//top left
	buffer[index++] = -0.5f; buffer[index++] = 0.5f;     buffer[index++] = 0.5f;		buffer[index++] = 0.0f;  buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = 0.5f;  buffer[index++] = -0.5f;    buffer[index++] = 0.5f;		buffer[index++] = 1.0f;  buffer[index++] = 0.0f;
	//bottom left
	buffer[index++] = -0.5f; buffer[index++] = -0.5f;    buffer[index++] = 0.5f;		buffer[index++] = 0.0f;  buffer[index++] = 0.0f;
	
	//top face
	//top right
	buffer[index++] = 0.5f;  buffer[index++] = 0.5f;     buffer[index++] = 0.5f;		buffer[index++] = 1.0f;  buffer[index++] = 1.0f;
	//top left
	buffer[index++] = -0.5f; buffer[index++] = 0.5f;     buffer[index++] = 0.5f;		buffer[index++] = 0.0f;  buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = 0.5f;  buffer[index++] = 0.5f;     buffer[index++] = -0.5f;		buffer[index++] = 1.0f;  buffer[index++] = 0.0f;
	//bottom left
	buffer[index++] = -0.5f; buffer[index++] = 0.5f;     buffer[index++] = -0.5f;		buffer[index++] = 0.0f;  buffer[index++] = 0.0f;
	


	//bottom face
	//top right
	buffer[index++] = 0.5f;  buffer[index++] = -0.5f;   buffer[index++] = 0.5f;		    buffer[index++] = 1.0f;  buffer[index++] = 1.0f;
	//top left
	buffer[index++] = -0.5f; buffer[index++] = -0.5f;   buffer[index++] = 0.5f;		    buffer[index++] = 0.0f;  buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = 0.5f;  buffer[index++] = -0.5f;   buffer[index++] = -0.5f;		buffer[index++] = 1.0f;  buffer[index++] = 0.0f;
	//bottom left
	buffer[index++] = -0.5f; buffer[index++] = -0.5f;   buffer[index++] = -0.5f;		buffer[index++] = 0.0f;  buffer[index++] = 0.0f;
	
	//left face
	//top right
	buffer[index++] = -0.5f; buffer[index++] = 0.5f;   buffer[index++] = -0.5f;			buffer[index++] = 1.0f;  buffer[index++] = 1.0f;
	//top left
	buffer[index++] = -0.5f; buffer[index++] = 0.5f;   buffer[index++] = 0.5f;			buffer[index++] = 0.0f;  buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = -0.5f; buffer[index++] = -0.5f;  buffer[index++] = -0.5f;			buffer[index++] = 1.0f;  buffer[index++] = 0.0f;
	//bottom left
	buffer[index++] = -0.5f; buffer[index++] = -0.5f;  buffer[index++] = 0.5f;			buffer[index++] = 0.0f;  buffer[index++] = 0.0f;

	//right face
	//top right
	buffer[index++] = 0.5f;  buffer[index++] = 0.5f;   buffer[index++] = 0.5f;			buffer[index++] = 1.0f;  buffer[index++] = 1.0f;
	//top left
	buffer[index++] = 0.5f;  buffer[index++] = 0.5f;   buffer[index++] = -0.5f;			buffer[index++] = 0.0f;  buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = 0.5f;  buffer[index++] = -0.5f;  buffer[index++] = 0.5f;			buffer[index++] = 1.0f;  buffer[index++] = 0.0f;
	//bottom left
	buffer[index++] = 0.5f;  buffer[index++] = -0.5f;  buffer[index++] = -0.5f;			buffer[index++] = 0.0f;  buffer[index++] = 0.0f;

	vertexArr.createVertexArray(sceneName);
	vertexBuff.createVertexBuffer(sceneName, 120 * sizeof(float), buffer);
	createVertexLayout<float>(sceneName, sceneName, 3, 2);

	tex.readTexture(sceneTexture, sceneName);
	
}
void Renderer::renderScene(const std::string& sceneName, int cubeCount, const std::string& texture) {
	glEnable(GL_DEPTH_TEST);
	if (vertices[sceneName].size() == 0  ) {
		if (texture.find("minecraft") != std::string::npos) {
			selectMinecraftTexture(texture);
			assignMinecraftBlockTexture(sceneName, 0.5f, 0.5f, 0.5f);
		}
		else if (texture == "wood") {
			sceneTexture = "wood.jpg";
			assignSceneCubeTexCoord(sceneName);
			
		}
		else if (texture == "steel") {
			sceneTexture = "stainless-steel.jpg";
			assignSceneCubeTexCoord(sceneName);
		}
		
		else if (texture == "brick") {
			sceneTexture = "brick.png";
			assignSceneCubeTexCoord(sceneName);
		}
		else {

			sceneTexture = "none";
			assignCubeVertices(sceneName, 0.5f, 0.5f, 0.5f, 1.0f, 0.5f, 0.2f, 1.0f);
		}
	}



	if (sceneName == "spiralScene") {
		renderSpiralScene(sceneName,  cubeCount);
	}
	else if (sceneName == "layeredScene") {
		renderLayeredScene(sceneName, cubeCount);
	}
	else {
		renderDefaultScene(sceneName, cubeCount);
	}
	
	
}
void Renderer::renderDefaultScene(const std::string& sceneName, int cubeCount) {
	std::string shaderSource;

	vertexArr.bind(sceneName);
	vertexBuff.bind(sceneName);
	indexBuff.bind("texturedCubeIndexBuffer");
	
	
	if (sceneTexture == "none") {
		shaderSource = "3DShader";
		indexBuff.bind("cubeIndexBuffer");
	}
	else {
		shaderSource = "TextureShader";
		tex.bind(sceneName);
		indexBuff.bind("texturedCubeIndexBuffer");
		shader.setUniform1f("TextureShader", "texUnit", 0.0f);
	}
	
	
	shader.bind(shaderSource);

	float* modelMat = vertices["cubeModelMatrix"].data();
	float* viewMat = vertices["cubeViewMatrix"].data();
	float* projMat = vertices["cubeProjectionMatrix"].data();

	shader.setUniformMatrix4fv(shaderSource, "view", viewMat);
	shader.setUniformMatrix4fv(shaderSource, "projection", projMat);

	// Initialize random seed based on scene name for consistent results
	std::hash<std::string> hasher;
	std::srand(static_cast<unsigned int>(hasher(sceneName)));

	// Create a grid-based system with randomness
	int gridSize = static_cast<int>(std::ceil(std::sqrt(cubeCount)));
	float baseSpacing = 2.5f; // Minimum distance between cube centers

	for (int i = 0; i < cubeCount; ++i) {
		setAsIdentityMatrix(modelMat);

		// Calculate grid position
		int gridX = i % gridSize;
		int gridY = i / gridSize;

		// Base position with some randomness
		float baseX = (gridX - gridSize * 0.5f) * baseSpacing;
		float baseY = (gridY - gridSize * 0.5f) * baseSpacing;
		float baseZ = 0.0f;

		// Add controlled randomness
		float randomX = (std::rand() % 200 - 100) / 100.0f; // -1.0 to 1.0
		float randomY = (std::rand() % 200 - 100) / 100.0f; // -1.0 to 1.0
		float randomZ = (std::rand() % 300 - 150) / 100.0f; // -1.5 to 1.5

		// Final positions
		float finalX = baseX + randomX;
		float finalY = baseY + randomY;
		float finalZ = baseZ + randomZ;

		// Random rotations
		float rotX = (std::rand() % 360) * M_PI / 180.0f;
		float rotY = (std::rand() % 360) * M_PI / 180.0f;
		float rotZ = (std::rand() % 360) * M_PI / 180.0f;

		// Random scale (subtle variation)
		float scale = 0.7f + (std::rand() % 60) / 100.0f; // 0.7 to 1.3

		// Apply transformations
		applyTransformations(modelMat, finalX, finalY, finalZ, rotX, rotY, rotZ, scale);

		// Draw cube
		shader.setUniformMatrix4fv(shaderSource, "model", modelMat);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)0);
	}
}
// Alternative: Spiral pattern with randomness
void Renderer::renderSpiralScene(const std::string& sceneName, int cubeCount) {
	std::string shaderSource;

	vertexArr.bind(sceneName);
	vertexBuff.bind(sceneName);

	if (sceneTexture == "none") {
		shaderSource = "3DShader";
		indexBuff.bind("cubeIndexBuffer");
	}
	else {
		shaderSource = "TextureShader";
		tex.bind(sceneName);
		indexBuff.bind("texturedCubeIndexBuffer");
		shader.setUniform1f("TextureShader", "texUnit", 0.0f);
	}

	shader.bind(shaderSource);

	float* modelMat = vertices["cubeModelMatrix"].data();
	float* viewMat = vertices["cubeViewMatrix"].data();
	float* projMat = vertices["cubeProjectionMatrix"].data();

	shader.setUniformMatrix4fv(shaderSource, "view", viewMat);
	shader.setUniformMatrix4fv(shaderSource, "projection", projMat);

	std::hash<std::string> hasher;
	std::srand(static_cast<unsigned int>(hasher(sceneName)));

	for (int i = 0; i < cubeCount; ++i) {
		setAsIdentityMatrix(modelMat);

		// Spiral pattern
		float angle = i * 0.5f; // Spiral angle
		float radius = 0.5f + i * 0.3f; // Expanding radius

		float spiralX = radius * std::cos(angle);
		float spiralY = radius * std::sin(angle);
		float spiralZ = i * -0.2f; // Move back as we spiral

		// Add randomness
		float randomX = (std::rand() % 120 - 60) / 100.0f; // -0.6 to 0.6
		float randomY = (std::rand() % 120 - 60) / 100.0f;
		float randomZ = (std::rand() % 120 - 60) / 100.0f;

		float finalX = spiralX + randomX;
		float finalY = spiralY + randomY;
		float finalZ = spiralZ + randomZ;

		// Random rotations
		float rotX = (std::rand() % 360) * M_PI / 180.0f;
		float rotY = (std::rand() % 360) * M_PI / 180.0f;
		float rotZ = (std::rand() % 360) * M_PI / 180.0f;

		// Varied scales
		float scale = 0.6f + (std::rand() % 80) / 100.0f; // 0.6 to 1.4

		applyTransformations(modelMat, finalX, finalY, finalZ, rotX, rotY, rotZ, scale);

		shader.setUniformMatrix4fv(shaderSource, "model", modelMat);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)0);
	}
}

// Alternative: Layered pattern
void Renderer::renderLayeredScene(const std::string& sceneName, int cubeCount) {
	std::string shaderSource;

	vertexArr.bind(sceneName);
	vertexBuff.bind(sceneName);

	if (sceneTexture == "none") {
		shaderSource = "3DShader";
		indexBuff.bind("cubeIndexBuffer");
	}
	else {
		shaderSource = "TextureShader";
		tex.bind(sceneName);
		indexBuff.bind("texturedCubeIndexBuffer");
		shader.setUniform1f("TextureShader", "texUnit", 0.0f);
	}


	shader.bind(shaderSource);

	float* modelMat = vertices["cubeModelMatrix"].data();
	float* viewMat = vertices["cubeViewMatrix"].data();
	float* projMat = vertices["cubeProjectionMatrix"].data();

	shader.setUniformMatrix4fv(shaderSource, "view", viewMat);
	shader.setUniformMatrix4fv(shaderSource, "projection", projMat);

	std::hash<std::string> hasher;
	std::srand(static_cast<unsigned int>(hasher(sceneName)));

	int cubesPerLayer = 8;

	for (int i = 0; i < cubeCount; ++i) {
		setAsIdentityMatrix(modelMat);

		int layer = i / cubesPerLayer;
		int posInLayer = i % cubesPerLayer;

		// Circular arrangement per layer
		float angle = (posInLayer * 2.0f * M_PI) / cubesPerLayer;
		float layerRadius = 2.0f + layer * 1.5f;

		float circleX = layerRadius * std::cos(angle);
		float circleY = layerRadius * std::sin(angle);
		float circleZ = -layer * 2.0f;

		// Add controlled randomness
		float randomX = (std::rand() % 100 - 50) / 100.0f; // -0.5 to 0.5
		float randomY = (std::rand() % 100 - 50) / 100.0f;
		float randomZ = (std::rand() % 100 - 50) / 100.0f;

		float finalX = circleX + randomX;
		float finalY = circleY + randomY;
		float finalZ = circleZ + randomZ;

		// Rotations based on layer and position
		float rotX = (angle + layer * 0.5f) + (std::rand() % 180 - 90) * M_PI / 180.0f;
		float rotY = (layer * 0.3f) + (std::rand() % 180 - 90) * M_PI / 180.0f;
		float rotZ = (posInLayer * 0.2f) + (std::rand() % 180 - 90) * M_PI / 180.0f;

		// Scale variation by layer
		float scale = 0.8f + (layer * 0.1f) + (std::rand() % 40) / 100.0f;

		applyTransformations(modelMat, finalX, finalY, finalZ, rotX, rotY, rotZ, scale);

		shader.setUniformMatrix4fv(shaderSource, "model", modelMat);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)0);
	}
}

// Helper method to apply all transformations
void Renderer::applyTransformations(float* modelMatrix, float x, float y, float z,
	float rotX, float rotY, float rotZ, float scale) {
	// Create transformation matrices
	float translationMatrix[16], scaleMatrix[16], rotationMatrix[16];
	float tempMatrix[16];

	// Translation matrix
	setAsIdentityMatrix(translationMatrix);
	translationMatrix[12] = x;
	translationMatrix[13] = y;
	translationMatrix[14] = z;

	// Scale matrix
	setAsIdentityMatrix(scaleMatrix);
	scaleMatrix[0] = scale;
	scaleMatrix[5] = scale;
	scaleMatrix[10] = scale;

	// Combined rotation matrix
	float* xRot = vertices["xRotation"].data();
	float* yRot = vertices["yRotation"].data();
	float* zRot = vertices["zRotation"].data();

	setAsIdentityMatrix(xRot);
	setAsIdentityMatrix(yRot);
	setAsIdentityMatrix(zRot);

	setXRotation(xRot, rotX);
	setYRotation(yRot, rotY);
	setZRotation(zRot, rotZ);

	// Combine rotations: X * Y * Z
	multiplyMatrices(xRot, yRot, tempMatrix);
	multiplyMatrices(tempMatrix, zRot, rotationMatrix);

	// Final transformation: Translation * Rotation * Scale
	multiplyMatrices(rotationMatrix, scaleMatrix, tempMatrix);
	multiplyMatrices(translationMatrix, tempMatrix, modelMatrix);
}


void Renderer::drawCube(const std::string& cubeName, float x, float y, float z, float r, float g, float b, float a) {
	if (vertices[cubeName].size() == 0) {
		assignCubeVertices(cubeName, x, y, z, r, g, b, a);		

	}

	 shader.bind("3DShader");
	vertexArr.bind(cubeName);
	vertexBuff.bind(cubeName);
	indexBuff.bind("cubeIndexBuffer");
	
	
	float* modelMat = vertices["cubeModelMatrix"].data();
	float* viewMat = vertices["cubeViewMatrix"].data();
	float* projectionMat = vertices["cubeProjectionMatrix"].data();

	shader.setUniformMatrix4fv("3DShader", "model", modelMat);
	shader.setUniformMatrix4fv("3DShader", "view", viewMat);
	shader.setUniformMatrix4fv("3DShader", "projection", projectionMat);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)0);

}
void Renderer::assignMinecraftBlockTexture(const std::string& blockName, float x, float y, float z) {
	vertices[blockName].resize(120);
	float* buffer = vertices[blockName].data();
	int index = 0;


	//back face
	//top right
	buffer[index++] = x, buffer[index++] = y, buffer[index++] = -z;
	buffer[index++] = 0.5f, buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = x, buffer[index++] = -y, buffer[index++] = -z;
	buffer[index++] = 0.5f, buffer[index++] = 0.0f;
	//top left
	buffer[index++] = -x, buffer[index++] = y, buffer[index++] = -z;
	buffer[index++] = 0.0f, buffer[index++] = 1.0f;
	//bottom left
	buffer[index++] = -x, buffer[index++] = -y, buffer[index++] = -z;
	buffer[index++] = 0.0f, buffer[index++] = 0.0f;
	

	//front face
	//top right 
	buffer[index++] = x, buffer[index++] = y, buffer[index++] = z;
	buffer[index++] = 0.5f, buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = x, buffer[index++] = -y, buffer[index++] = z;
	buffer[index++] = 0.5f, buffer[index++] = 0.0f;
	//top left
	buffer[index++] = -x, buffer[index++] = y, buffer[index++] = z;
	buffer[index++] = 0.0f, buffer[index++] = 1.0f;
	//bottom left
	buffer[index++] = -x, buffer[index++] = -y, buffer[index++] = z;
	buffer[index++] = 0.0f, buffer[index++] = 0.0f;


	//bottom face
	//top right
	buffer[index++] = x, buffer[index++] = -y, buffer[index++] = -z;
	buffer[index++] = 0.5f, buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = x, buffer[index++] = -y, buffer[index++] = z;
	buffer[index++] = 0.5f, buffer[index++] = 0.0f;
	//top left
	buffer[index++] = -x, buffer[index++] = -y, buffer[index++] = -z;
	buffer[index++] = 0.0f, buffer[index++] = 1.0f;
	//bottom left 
	buffer[index++] = -x, buffer[index++] = -y, buffer[index++] = z;
	buffer[index++] = 0.0f, buffer[index++] = 0.0f;

	//top face
	//top right
	buffer[index++] = x, buffer[index++] = y, buffer[index++] = z;
	buffer[index++] = 1.0f, buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = x, buffer[index++] = y, buffer[index++] = -z;
	buffer[index++] = 1.0f, buffer[index++] = 0.0f;
	//top left
	buffer[index++] = -x, buffer[index++] = y, buffer[index++] = z;
	buffer[index++] = 0.5f, buffer[index++] = 1.0f;
	//bottom left 
	buffer[index++] = -x, buffer[index++] = y, buffer[index++] = -z;
	buffer[index++] = 0.5f, buffer[index++] = 0.0f;


	//left face
	//top right 
	buffer[index++] = -x, buffer[index++] = y, buffer[index++] = z;
	buffer[index++] = 0.5f, buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = -x, buffer[index++] = -y, buffer[index++] = z;
	buffer[index++] = 0.5f, buffer[index++] = 0.0f;
	//top left
	buffer[index++] = -x, buffer[index++] = y, buffer[index++] = -z;
	buffer[index++] = 0.0f, buffer[index++] = 1.0f;
	//bottom left
	buffer[index++] = -x, buffer[index++] = -y, buffer[index++] = -z;
	buffer[index++] = 0.0f, buffer[index++] = 0.0f;
	

	//right face
	//top right
	buffer[index++] = x, buffer[index++] = y, buffer[index++] = -z;
	buffer[index++] = 0.5f, buffer[index++] = 1.0f;
	//bottom right
	buffer[index++] = x, buffer[index++] = -y, buffer[index++] = -z;
	buffer[index++] = 0.5f, buffer[index++] = 0.0f;
	//top left 
	buffer[index++] = x, buffer[index++] = y, buffer[index++] = z;
	buffer[index++] = 0.0f, buffer[index++] = 1.0f;
	//bottom left
	buffer[index++] = x, buffer[index++] = -y, buffer[index++] = z;
	buffer[index++] = 0.0f, buffer[index++] = 0.0f;


	vertexArr.createVertexArray(blockName);
	vertexBuff.createVertexBuffer(blockName, 120 * sizeof(float), buffer);
	createVertexLayout<float>(blockName, blockName, 3, 2);
	
	tex.readTexture(minecraftTexture, blockName);
}

void Renderer::normalizeVector(float& x, float& y, float& z) {
	float length = std::sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;

}

void Renderer::createTexturedCubeIndexBuffer() {
	unsigned int texturedCubeIndices[]{
		//back face
		0, 1, 2,
		3, 1, 2,

		//front face
		4, 5, 6,
		7, 5, 6,

		//buttom face
		8, 9, 10,
		11, 9, 10,

		//top face
		12, 13, 14,
		15, 13, 14,

		//left face
		16, 17, 18,
		19, 17, 18,

		//right face
		20, 21, 22,
		23, 21, 22


	};
	indices["texturedCubeIndexBuffer"].resize(36);
	unsigned int* blockIndices = indices["texturedCubeIndexBuffer"].data();
	for (int i = 0; i < 36; ++i) {
		blockIndices[i] = texturedCubeIndices[i];
	}

	indexBuff.createIndexBuffer("texturedCubeIndexBuffer", 36 * sizeof(unsigned int), blockIndices);
}

void Renderer::selectMinecraftTexture(const std::string & blockName) {
	if (blockName == "minecraftGrass"){
		minecraftTexture = "block-grass-texture-atlas.png";
	}
	else if (blockName == "minecraftStone") {
		minecraftTexture = "minecraft-stone-texture-atlas.png";
	}
	else if (blockName == "minecraftGlass") {
		minecraftTexture = "minecraft-glass-texture-atlas.png";
	}
	else {
		minecraftTexture = "block-grass-texture-atlas.png";
	}

		
		
}

void Renderer::drawMinecraftBlock(const std::string& blockName, float x, float y, float z) {
	if (vertices[blockName].size() == 0) {
		selectMinecraftTexture(blockName);
		assignMinecraftBlockTexture(blockName, x, y, z);
		
	}
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	vertexArr.bind(blockName);
	vertexBuff.bind(blockName);
	indexBuff.bind("texturedCubeIndexBuffer");
	tex.bind(blockName);
	shader.bind("TextureShader");

	// Set texture uniform
	shader.setUniform1f("TextureShader", "texUnit", 0);

	// Apply rotation to the cube (not the camera)
	float* modelMat = vertices["cubeModelMatrix"].data();
	setAsIdentityMatrix(modelMat);
	setYRotation(modelMat, glfwGetTime() * 0.5f); // Slower rotation
	shader.setUniformMatrix4fv("TextureShader", "model", modelMat);

	// Use the updated view matrix (camera is handled automatically)
	float* viewMat = vertices["cubeViewMatrix"].data();
	float* projMat = vertices["cubeProjectionMatrix"].data();
	shader.setUniformMatrix4fv("TextureShader", "view", viewMat);
	shader.setUniformMatrix4fv("TextureShader", "projection", projMat);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)0);

}





void Renderer::enableCamera(GLFWwindow* window) {
	if (!cursorIsHidden) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		cursorIsHidden = true;
	}

	static double startTime = glfwGetTime();
	double currentTime = glfwGetTime();

	float deltaTime = static_cast<float>(currentTime - startTime);
	startTime = currentTime;

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);


	if (firstMouse) {
		lastMouseX = mouseX;
		lastMouseY = mouseY;
		firstMouse = false;
	}

	float offsetX = mouseX - lastMouseX;
	float offsetY = lastMouseY - mouseY;

	offsetX *= mouseSensitivity;
	offsetY *= mouseSensitivity;

	lastMouseX = mouseX;
	lastMouseY = mouseY;

	yaw += offsetX;
	pitch += offsetY;

	//To prevent screen flipping
	if (pitch > 89.0f)  pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	assignForwardView(window, deltaTime);

}
void Renderer::assignForwardView(GLFWwindow* window, float& deltaTime) {
	float yawRad = yaw * M_PI / 180.0f;
	float pitchRad = pitch * M_PI / 180.0f;

	forwardX = std::cos(yawRad) * std::cos(pitchRad);
	forwardY = std::sin(pitchRad);
	forwardZ = std::sin(yawRad) * std::cos(pitchRad);
	normalizeVector(forwardX, forwardY, forwardZ);	
	
	updateViewMatrix(window, deltaTime);
}

void Renderer::updateViewMatrix(GLFWwindow* window, float& deltaTime) {
	float* viewMat = vertices["cubeViewMatrix"].data();

	float rightX = forwardY * cameraUpZ - forwardZ * cameraUpY;
	float rightY = forwardZ * cameraUpX - forwardX * cameraUpZ;
	float rightZ = forwardX * cameraUpY - forwardY * cameraUpX;
	normalizeVector(rightX, rightY, rightZ);

	float upX = rightY * forwardZ - rightZ * forwardY;
	float upY = rightZ * forwardX - rightX * forwardZ;
	float upZ = rightX * forwardY - rightY * forwardX;
	normalizeVector(upX, upY, upZ);

	float velocity = cameraSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//viewMat[14] += -0.1f * deltaTime;
		cameraX += forwardX * velocity;
		cameraY += forwardY * velocity;
		cameraZ += forwardZ * velocity;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraX -= forwardX * velocity;
		cameraY -= forwardY * velocity;
		cameraZ -= forwardZ * velocity;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraX -= rightX * velocity;
		cameraY -= rightY * velocity;
		cameraZ -= rightZ * velocity;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraX += rightX * velocity;
		cameraY += rightY * velocity;
		cameraZ += rightZ * velocity;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cameraY += velocity;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cameraY -= velocity;
	}
	
	float rightTranlation = -(rightX * cameraX + rightY * cameraY + rightZ * cameraZ);
	float upTranslation = -(upX * cameraX + upY * cameraY + upZ * cameraZ);
	float forwardTranslation = -(-forwardX * cameraX + -forwardY * cameraY + -forwardZ * cameraZ);

	
	
	viewMat[0] = rightX;	 viewMat[4] = rightY;		viewMat[8] = rightZ;		viewMat[12] = rightTranlation;
	viewMat[1] = upX;	     viewMat[5] = upY;			viewMat[9] = upZ;			viewMat[13] = upTranslation;
	viewMat[2] = -forwardX;	 viewMat[6] = -forwardY;	viewMat[10] = -forwardZ;	viewMat[14] = forwardTranslation;
	viewMat[3] = 0.0f;		 viewMat[7] = 0.0f;			viewMat[11] = 0.0f;			viewMat[15] = 1.0f;

}
void Renderer::createCubeIndexBuffer() {
	indices["cubeIndexBuffer"].resize(36);
	
	unsigned int cubeIndexBuff[36]{
		//top
		0, 1, 2,
		0, 2, 3,

		//bottom
		4, 5, 6,
		4, 6, 7,

		//front
		1, 2, 5,
		5, 2, 6,

		//back
		0, 3, 4,
		3, 4, 7,

		//left
		0, 1, 5,
		0, 4, 5,

		//right
		2, 3, 6,
		3, 6,  7
	};


	unsigned int* buffer = indices["cubeIndexBuffer"].data();
	for (int i = 0; i < 36; ++i) {
		buffer[i] = cubeIndexBuff[i];
	}

	indexBuff.createIndexBuffer("cubeIndexBuffer", 36 * sizeof(unsigned int), buffer);
}



void Renderer::multiplyMatrices(float* matrixA, float* matrixB, float * destMatrix) {

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			destMatrix[row * 4 + col] = matrixA[row * 4 + 0] * matrixB[0 + col] +
						         		matrixA[row * 4 + 1] * matrixB[4 + col] +
										matrixA[row * 4 + 2] * matrixB[8 + col] +
										matrixA[row * 4 + 3] * matrixB[12 + col];

		
		}

		
	}

	 

}

void Renderer::setXRotation(float * matrix, float angle) {
		/*
	float xRotation[]{
		1,   0,      0,         0,
		0, cosAngle, -sinAngle,  0,
		0, sinAngle, cosAngle, 0,
		0,  0,        0,        1

	};
	*/

	

	matrix[5] = std::cos(angle);
	matrix[6] = -std::sin(angle);
	matrix[9] = std::sin(angle);
	matrix[10] = std::cos(angle);


}

void Renderer::setYRotation(float* matrix, float angle) {
	/*
	float yRotation[]{
		cosAngle, 0, sinAngle, 0,
		0,        1,    0,     0,
	   -sinAngle, 0,  cosAngle, 0,
		0,        0,     0,     1
	};

	*/

	matrix[0] = std::cos(angle);
	matrix[2] = std::sin(angle);
	matrix[8] = -std::sin(angle);
	matrix[10] = std::cos(angle);

}

void Renderer::setZRotation(float* matrix, float angle) {
	/*
	float zRotation[]{
		cosAngle, -sinAngle, 0, 0,
		sinAngle, cosAngle, 0, 0,
		 0,         0,      1,  0,
		 0,         0,      0,   1
	}; */


	matrix[0] = std::cos(angle);
	matrix[1] = -std::sin(angle);
	matrix[4] = std::sin(angle);
	matrix[5] = std::cos(angle);

}


float xRotationAngle = 0.0f;
float yRotationAngle = 0.0f;
float zRotationAngle = 0.0f;
void Renderer::controlCube(GLFWwindow* window, const std::string& cubeName) {
	static double previousTime = glfwGetTime();
	double currentTime = glfwGetTime();

	float deltaTime = static_cast<float>(currentTime - previousTime);
	previousTime = currentTime;

	float rotationSpeed = 3.0f;
	
	static float targetXAngle = 0.0f;
	static float targetYAngle = 0.0f;
	static float targetZAngle = 0.0f;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		targetZAngle += 2.0f * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		targetZAngle -= 2.0f * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		targetXAngle -= 2.0f * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		targetXAngle += 2.0f * deltaTime;
	}


	xRotationAngle += (targetXAngle - xRotationAngle) * rotationSpeed * deltaTime;
	zRotationAngle += (targetZAngle - zRotationAngle) * rotationSpeed * deltaTime;

	float* modelMat = vertices["cubeModelMatrix"].data();
	setAsIdentityMatrix(modelMat);

	float * xRotationMat = vertices["xRotation"].data();
	float * zRotationMat = vertices["zRotation"].data();
	setAsIdentityMatrix(xRotationMat);
	setAsIdentityMatrix(zRotationMat);

	setXRotation(xRotationMat, xRotationAngle);
	setZRotation(zRotationMat, zRotationAngle);
	multiplyMatrices(zRotationMat, xRotationMat, modelMat);

	
	vertexArr.bind(cubeName);
	vertexBuff.bind(cubeName);
	indexBuff.bind("cubeIndexBuffer");

	shader.bind("3DShader");
	shader.setUniformMatrix4fv("3DShader", "model", modelMat);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)0);


	
}
void Renderer::rotateCubeYZ(const std::string& cubeName) {
	vertexArr.bind(cubeName);
	vertexBuff.bind(cubeName);
	indexBuff.bind("cubeIndexBuffer");

	float* modelMat = vertices["cubeModelMatrix"].data();
	double angle = glfwGetTime();
	
	float cosAngle = std::cos(angle);
	float sinAngle = std::sin(angle);

	float* yRotation = vertices["yRotation"].data();
	float* zRotation = vertices["zRotation"].data();
	


	setYRotation(yRotation, angle);
	setZRotation(zRotation, angle);

	setAsIdentityMatrix(modelMat);
	multiplyMatrices(yRotation, zRotation, modelMat);

	shader.bind("3DShader");
	glUniformMatrix4fv(shader.findUniformLocation("3DShader", "model"), 1, GL_FALSE, modelMat);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)0);



}

void Renderer::rotateCubeXYZ(const std::string& cubeName) {
	vertexArr.bind(cubeName);
	vertexBuff.bind(cubeName);
	indexBuff.bind("cubeIndexBuffer");

	float* modelMat = vertices["cubeModelMatrix"].data();
	double angle = glfwGetTime();

	float cosAngle = std::cos(angle);
	float sinAngle = std::sin(angle);

	float* xRotation = vertices["xRotation"].data();
	float* yRotation = vertices["yRotation"].data();
	float* zRotation = vertices["zRotation"].data();

	setXRotation(xRotation, angle);
	setYRotation(yRotation, angle);
	setZRotation(zRotation, angle);

	float xyRotation[16];

	setAsIdentityMatrix(modelMat);
	multiplyMatrices(xRotation, yRotation, xyRotation);
	multiplyMatrices(xyRotation, zRotation, modelMat);

	shader.bind("3DShader");
	glUniformMatrix4fv(shader.findUniformLocation("3DShader", "model"), 1, GL_FALSE, modelMat);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)0);



}



void Renderer::rotateCubeY(const std::string& cubeName) {
	vertexArr.bind(cubeName);
	vertexBuff.bind(cubeName);
	indexBuff.bind("cubeIndexBuffer");

	float* modelMat = vertices["cubeModelMatrix"].data();
	
	double angle = glfwGetTime();
	setYRotation(modelMat, angle);
	shader.bind("3DShader");

	glUniformMatrix4fv(shader.findUniformLocation("3DShader", "model"), 1, GL_FALSE, modelMat);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)0);
}



void Renderer::rotateCubeZ(const std::string& cubeName) {
	vertexArr.bind(cubeName);
	vertexBuff.bind(cubeName);
	indexBuff.bind("cubeIndexBuffer");

	float* modelMat = vertices["cubeModelMatrix"].data();

	double angle = glfwGetTime();
	setZRotation(modelMat, angle);

	shader.bind("3DShader");

	glUniformMatrix4fv(shader.findUniformLocation("3DShader", "model"), 1, GL_FALSE, modelMat);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)0);


}

void Renderer::rotateCubeX(const std::string& cubeName) {
	vertexArr.bind(cubeName);
	vertexBuff.bind(cubeName);
	indexBuff.bind("cubeIndexBuffer");

	float* modelMat = vertices["cubeModelMatrix"].data();

	double angle = glfwGetTime();
	setXRotation(modelMat, angle);

	shader.bind("3DShader");

	glUniformMatrix4fv(shader.findUniformLocation("3DShader", "model"), 1, GL_FALSE, modelMat);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)0);

}

void Renderer::setAsIdentityMatrix(float* matrix) {
	for (int row = 0; row < 4; ++row) {
		
		for (int col = 0; col < 4; ++col) {
			matrix[row * 4 + col] = 0.0f;
		}
		matrix[row * 5] = 1.0f;
	}
}
void Renderer::initMatrices() {
	float* cubeModelMat = vertices["cubeModelMatrix"].data();
	float* cubeViewMat = vertices["cubeViewMatrix"].data();
	float* cubeProjectMat = vertices["cubeProjectionMatrix"].data();
	
	float* xRotation = vertices["xRotation"].data();
	float* yRotation = vertices["yRotation"].data();
	float* zRotation = vertices["zRotation"].data();

	
	setAsIdentityMatrix(cubeModelMat);
	setAsIdentityMatrix(cubeViewMat);
	setAsIdentityMatrix(cubeProjectMat);

	
	setAsIdentityMatrix(xRotation);
	setAsIdentityMatrix(yRotation);
	setAsIdentityMatrix(zRotation);



float near = 0.10f;
float far = 100.00f;
float farNearPlane = -(far + near) / (far - near);
float planeDistance = -(2 * far * near) / (far - near);

float projectionMat[16]{
	1, 0,  0,       0,
	0, 1,  0,       0,
	0, 0, farNearPlane, -1.0,
	0, 0, planeDistance,     0
};



cubeProjectMat[10] = farNearPlane;
cubeProjectMat[14] = planeDistance;
cubeProjectMat[11] = -1.0f;
cubeProjectMat[15] = 0.0f;


//Move cube back
cubeViewMat[14] = -1.5f;

	shader.bind("3DShader");

	glUniformMatrix4fv(shader.findUniformLocation("3DShader", "model"), 1, GL_FALSE, cubeModelMat);
	glUniformMatrix4fv(shader.findUniformLocation("3DShader", "view"),  1, GL_FALSE, cubeViewMat);
	glUniformMatrix4fv(shader.findUniformLocation("3DShader", "projection"), 1, GL_FALSE, cubeProjectMat);

}
Renderer::~Renderer() {
	
}

