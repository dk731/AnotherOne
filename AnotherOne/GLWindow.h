#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

class GLWindow
{

public:
	int myW, myH;
	bool initialized = false;

	GLFWwindow *windowHandle = nullptr;

	GLWindow(int width, int height, std::string winName):
		myW(width), myH(height)
	{
		if (!glfwInit())
		{
			std::cout << "Was not able to initialize window." << std::endl;
			return;
		}

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		windowHandle = glfwCreateWindow(myW, myH, winName.c_str(), nullptr, nullptr);

		if (!windowHandle)
		{
			std::cout << "Was not able to create window." << std::endl;
			return;
		}

		glfwMakeContextCurrent(windowHandle);
		glfwSetInputMode(windowHandle, GLFW_STICKY_KEYS, GL_TRUE);
	}
};

