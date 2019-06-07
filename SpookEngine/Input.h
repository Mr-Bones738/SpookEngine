#ifndef _H_INPUT_
#define _H_INPUT_
#include <GLFW/glfw3.h>
class Input {
public:
	bool GetKeyDown(const unsigned int& key);
	bool GetKey(const unsigned int& key);
	bool GetKeyUp(const unsigned int& key);
	glm::dvec2 GetMouseDelta();
	friend class Engine;
private:
	static Input& Instance() {
		static Input _inst;
		return _inst;
	}
	static void GLFWKeyCallback(GLFWwindow* wind, int key, int, int action, int);
	static void GLFWMousePosCallback(GLFWwindow* wind, double x, double y);
	static void GLFWMouseButtonCallback(GLFWwindow* wind, int button, int action, int mods);
	void InternalKeyCallback(GLFWwindow* wind, int key, int, int action, int);
	void InternalMousePosCallback(GLFWwindow* wind, double x, double y);
	void InternalMouseButtonCallback(GLFWwindow* wind, int button, int action, int mods);
	void Frame();
	bool keys[512];
	bool lkeys[512];
	bool nkeys[512];
	double x;
	double y;
	double lastx;
	double lasty;
	double nx;
	double ny;
};
#endif