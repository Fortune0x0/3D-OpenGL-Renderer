#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include "Renderer.h"
#include <istream>
#define SCREEN_WIDTH 880
#define SCREEN_HEIGHT 680

int main() {
	if (!glfwInit()) {
		std::cerr << "GLFW WAS NOT INITIALZED\n";
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Renderer", NULL, NULL);

	if (!window) {
		std::cerr << "WINDOW WAS NOT CREATED>>\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW WAS NOT INITIALIZED\n";
		glfwTerminate();
		return -1;
	}


	const int FPS = 60;
	int frames = 0;
	double frameDrawTime = 1000.0 / FPS;
	double elapsedTime = 0.0;
	double timer = 0.0;
	auto startTime = std::chrono::steady_clock::now();

	Renderer renderer;
	renderer.drawCircle("Circle", 0.0f, 0.0f, 0.1f, 360, 0.3f, 0.4f, 0.5f, 1.0f);
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto currentTime = std::chrono::steady_clock::now();
		double deltaTime = std::chrono::duration<double, std::milli>(currentTime - startTime).count();
		elapsedTime += deltaTime;
		timer += deltaTime;

		startTime = currentTime;

		if (elapsedTime >= frameDrawTime) {
			elapsedTime -= frameDrawTime;
			++frames;
		}

		if (timer >= 1000.0) {
			std::cout << "FPS: " << frames << "\n";
			frames = 0;
			timer -= 1000.0;
		}
		
		renderer.renderScene("coolScene", 100, "minecraftGlass");
		renderer.enableCamera(window);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}


}