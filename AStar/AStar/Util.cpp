#include <iostream>

//#include <glad\glad.h>

#include "Util.h"

//void errorCallback(int error, const char* description)
//{
//	fprintf(stderr, "Error: %s\n", description);
//}
//
//void framebufferSizeCallback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}
//
//void Util::initWindowAndContext(GLFWwindow *& window, const std::string & windowTitle)
//{
//	// Setup error handling callback
//	glfwSetErrorCallback(errorCallback);
//
//	if (!glfwInit())
//		exit(EXIT_FAILURE);
//
//	// Create opengl window and context
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	window = glfwCreateWindow(800, 800, windowTitle.c_str(), nullptr, nullptr);
//	if (!window) {
//		std::cerr << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		exit(EXIT_FAILURE);
//	}
//
//	// Register callbacks
//	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
//
//	// Load opengl functinos
//	glfwMakeContextCurrent(window);
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//		std::cerr << "Failed to initialize GLAD" << std::endl;
//		glfwTerminate();
//		exit(EXIT_FAILURE);
//	}
//
//	// Configure context
//	glfwSwapInterval(1);
//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//
//	// Setup opengl viewport
//	int width, height;
//	glfwGetFramebufferSize(window, &width, &height);
//	glViewport(0, 0, width, height);
//}
