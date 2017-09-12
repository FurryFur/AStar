#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <string>

#include <GLFW\glfw3.h>

namespace Util {
	void initWindowAndContext(GLFWwindow*& window, const std::string& windowTitle);
}

#endif // UTIL_H