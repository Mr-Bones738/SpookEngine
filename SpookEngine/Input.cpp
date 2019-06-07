#include "Engine.h"
#include "Input.h"

bool Input::GetKeyDown(const unsigned int& key) {
	return keys[key] && !lkeys[key];
}

bool Input::GetKey(const unsigned int& key) {
	return keys[key];
}

bool Input::GetKeyUp(const unsigned int& key) {
	return !keys[key] && lkeys[key];
}

glm::dvec2 Input::GetMouseDelta() {
	return glm::dvec2(x - lastx, y - lasty);
}

void Input::GLFWKeyCallback(GLFWwindow* wind, int key, int scan, int action, int mods) {
	Input::Instance().InternalKeyCallback(wind, key, scan, action, mods);
}

void Input::GLFWMousePosCallback(GLFWwindow* wind, double x, double y) {
	Input::Instance().InternalMousePosCallback(wind, x, y);
}

void Input::GLFWMouseButtonCallback(GLFWwindow* wind, int button, int action, int mods) {
	Input::Instance().InternalMouseButtonCallback(wind, button, action, mods);
}

void Input::InternalKeyCallback(GLFWwindow* wind, int key, int scan, int action, int mods) {
	if (action == GLFW_PRESS) {
		nkeys[key] = true;
	}
	if (action == GLFW_RELEASE) {
		nkeys[key] = false;
	}
}

void Input::InternalMousePosCallback(GLFWwindow* wind, double x, double y) {
	nx = x;
	ny = y;
}

void Input::InternalMouseButtonCallback(GLFWwindow* wind, int button, int action, int mods) {

}

void Input::Frame() {
	memcpy(lkeys, keys, 512 * sizeof(bool));
	memcpy(keys, nkeys, 512 * sizeof(bool));
	lastx = x;
	lasty = y;
	x = nx;
	y = ny;
}