#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstdio>

#include "Engine/Rendering.hpp"
#include "Game/Game.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void errorCallback(int /*error*/, const char* description) { printf("Error: %s\n", description); };

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
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	Game game(window);

	double currentTime = glfwGetTime();
	double previousTime = currentTime;
	double dt = 0.0;

	float fps = 0.0f;
	float tempFps = 0.0f;
	int counter = 0;
	float fpsUpdate = 1.0f;
	float fpsUpdateTimer = 0.0f;

    double minUpdateRate = 1.0f / 120.0f;
	double updateTimer = 0.0f;
	int updatesSinceRender = 0;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

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

		updateTimer += dt;
		updatesSinceRender = 0;

        // If dt is bigger than minUpdateRate - update multiple times
        while (updateTimer >= minUpdateRate) { 
			if (updatesSinceRender >= 20) {
                // Too many updates, throw away the rest of dt (makes the game run in slow-motion)
				updateTimer = 0.0f;
				break;
			}

		    game.update((float) minUpdateRate);
            updateTimer -= minUpdateRate;
            updatesSinceRender++;
        }

        if (updatesSinceRender == 0) { // dt is faster than 
            game.update((float) updateTimer);
            updateTimer = 0.0f;
        }
		Rendering::getInstance().update((float) dt);

        // render
        // ------
        Rendering::getInstance().draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *theWindow)
{
    if(glfwGetKey(theWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(theWindow, true);
    }
}

void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height)
{
     glViewport(0, 0, width, height);
}
