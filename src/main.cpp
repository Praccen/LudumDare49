#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstdio>

#include "Engine/Rendering.hpp"
#include "Game/Game.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void errorCallback(int error, const char* description) { printf("Error: %s\n", description); };

GLFWwindow* window;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    glfwSetErrorCallback(errorCallback);
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    Rendering rendering;
	Game game(rendering, window);

	double currentTime = glfwGetTime();
	double previousTime = currentTime;
	double dt = 0.0;

	float fps = 0.0f;
	float tempFps = 0.0f;
	int counter = 0;
	float fpsUpdate = 1.0f;
	float fpsUpdateTimer = 0.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
		game.processInput(window);

		// Update
		// -----
		currentTime = glfwGetTime();
		dt = currentTime - previousTime;
		previousTime = currentTime;

		if (fpsUpdateTimer <= fpsUpdate) {
			fpsUpdateTimer += (float) dt;
			tempFps += (1 / (float) dt);
			counter++;
		}
		else {
			fps = tempFps / counter;
			tempFps = 0.0f;
			counter = 0;
			fpsUpdateTimer = 0.0f;
			//std::cout << fps;
			glfwSetWindowTitle(window, ("OpenGL FPS: " + std::to_string((int)fps)).c_str());
		}

		game.update((float)dt);
		rendering.update((float) dt);

        // render
        // ------
        rendering.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
     glViewport(0, 0, width, height);
}
