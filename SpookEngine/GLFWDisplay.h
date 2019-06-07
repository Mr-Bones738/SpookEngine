#ifndef _H_GLFWDISPLAY_
#define _H_GLFWDISPLAY_
#include "OpenGLRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLFWDisplay {
public:
	GLFWDisplay();
	OpenGLRenderer& GetRenderer();
	GLFWwindow* GetWindow();
	void Init();
	void Render();
private:
	struct Impl;
	Impl* pImpl;
};
#endif