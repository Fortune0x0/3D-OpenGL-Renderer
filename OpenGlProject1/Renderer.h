#pragma once
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include <GLFW/glfw3.h>
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include <string>





class Renderer {
public:
	Renderer();
	~Renderer();

	
	void enableCamera(GLFWwindow* window);
	void renderScene(const std::string& sceneName, int cubeCount, const std::string& texture = "none");
	void drawCircle(const std::string & circleName = "Circle", float centerX = 0.0f, float centerY = 0.0f, float radius = 0.5f, int res = 360, float f1 = 0.0f, float f2 = 0.0f, float f3 = 0.0f, float f4 = 1.0f);

	void drawTriangle(const std::string & triangleName = "Triangle", float x1 = 0.0f, float y1 = 0.3f, float x2 = -0.3f, float y2 = -0.3f, float x3 = 0.3f, float y3 = -0.3f,
		float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
		
	void rotateShape(const std::string& shape, float radius, float offset);
	
	void rotate(const std::string& shape, float radius, float offset);

	void drawTriangleTex(const std::string& texpath);

	
	void drawCube(const std::string& cubeName = "cube", float x = 0.5f, float y = 0.5f, float z = 0.5f, float r = 1.0f, float g = 0.5f, float b = 0.2f, float a = 1.0f);
	void drawMinecraftBlock(const std::string& blockName, float x = 0.5f, float y = 0.5f, float z = 0.5f);

	void rotateCubeX(const std::string & cubeName = "cube");
	void rotateCubeY(const std::string& cubeName = "cube");
	void rotateCubeZ(const std::string& cubeName = "cube");
	void rotateCubeYZ(const std::string& cubeName = "cube");
	void rotateCubeXYZ(const std::string& cubeName = "cube");

	 
	void controlCube(GLFWwindow* window, const std::string& cubeName = "cube");

private:
	void selectMinecraftTexture(const std::string& blockName);
	void normalizeVector(float& x, float& y, float& z);
	void assignForwardView(GLFWwindow * window, float & deltaTime);
	void updateViewMatrix(GLFWwindow* window, float& deltaTime);
	void createTexturedCubeIndexBuffer();
	void setXRotation(float* matrix, float angle);
	void setYRotation(float* matrix, float angle);
	void setZRotation(float* matrix, float angle);
	void setAsIdentityMatrix(float* matrix);
	void multiplyMatrices(float* matrixA, float* matrixB, float * destMatrix);
	void createCubeIndexBuffer();
	void initMatrices();
	void assignTriangleTex(const std::string& triangleName);
	void assignCubeVertices(const std::string& cubeName, float x, float y , float z ,  float r, float g, float b, float a);
	void assignSceneCubeTexCoord(const std::string& sceneName);
	void assignMinecraftBlockTexture(const std::string& blockName, float x, float y, float z);
	template<typename T>
	void addCounts(const std::string& vbName, unsigned int count);

	template<typename T, typename... Args>
	void createVertexLayout(const std::string vaName, const std::string& vbName, Args... counts);

	void renderSpiralScene(const  std::string& sceneName,  int cubeCount);
	void renderLayeredScene(const std::string& sceneName,  int cubeCount);
	void renderDefaultScene(const std::string& sceneName,  int cubeCount);

	void assignCircleVertices(const std::string& circleName, const float& centerX = 0.0f, const float& centerY = 0.0f, const float& radius = 0.5f, const int& res = 360, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void assignTriangleVertices(const std::string& triangleName, float x1 = 0.0f, float y1 = 0.3f, float x2 = -0.3f, float y2 = -0.3f, float x3 = 0.3f, float y3 = -0.3f,
		float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);



	void applyTransformations(float* modelMatrix, float x, float y, float z,
		float rotX, float rotY, float rotZ, float scale);


	float yaw = -90.0f;
	float pitch = 0.0f;

	float forwardX, forwardY, forwardZ;

	float cameraUpX = 0.0f;
	float cameraUpY = 1.0f;
	float cameraUpZ = 0.0f;

	float cameraX = 0.0f;
	float cameraY = 0.0f;
	float cameraZ = 5.0f;

	float mouseSensitivity = 0.1f;
	float cameraSpeed = 2.5f;

	bool firstMouse = true;
	double lastMouseX, lastMouseY;





	
	float translationUnit = 0.5f;
	

	bool cursorIsHidden = false;
	std::string sceneTexture, minecraftTexture;
	VertexBuffer vertexBuff;
	IndexBuffer indexBuff;
	Texture tex;
	Shader shader;
	VertexBufferLayout vertexLayout;
	VertexArray vertexArr;

	std::unordered_map<std::string, std::vector<unsigned int>>indices;
	std::unordered_map<std::string, std::vector<float>> vertices;
};