#ifndef _H_ENGINE_
#define _H_ENGINE_
#include "IO.h"
#include "GLFWDisplay.h"
#include "OpenGLRenderer.h"
#include "OpenGLShaderManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "World.h"
#include "Input.h"
class Engine {
public:
	static Engine& Instance() {
		static Engine _inst;
		return _inst;
	}
	SEIO& GetIO();
	OpenGLRenderer& GetRenderer();
	GLFWDisplay& GetDisplay();
	OpenGLShaderManager& GetShaderMan();
	TextureManager& GetTextureMan();
	ModelManager& GetModelMan();
	World& GetActiveWorld();
	Input& GetInput();
	void Init();
	bool Update();
	float GetDeltaTime() const;
	int GetWindWidth() const;
	int GetWindHeight() const;
private:
	struct Impl;
	Impl* pImpl; // opaque pointer to prevent files that include this (most) from recompiling
	static void GLFWErrorCallback(int errorcode, const char* msg);
	static void GLFWKeyCallback(GLFWwindow* wind, int key, int, int action, int);
	static void GLFWCursorPosCallback(GLFWwindow*, double x, double y);
	static void GLFWMouseButtonCallback(GLFWwindow*, int button, int action, int);
	double mouseX;
	double mouseY;
	Engine();
	~Engine() = default;
	Engine(const Engine&) = delete;
};

#endif