#include <iostream>
#include <string>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "Util.h"

int main()
{
	GLFWwindow* window;

	Util::initWindowAndContext(window, "AStar");

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}