#pragma once
#include "Component.h"
#include <GLFW/glfw3.h>

class InputComponent: public Component
{
public:
	int keys[1024];

	InputComponent(GLFWwindow* window);

	void calculateInput();

private:
	GLFWwindow* m_window;
};

