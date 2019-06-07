#include "Engine.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <chrono>
#include "OpenGLShaderManager.h"
struct Engine::Impl {
	float lastTime;
	float deltaTime;
	GLFWDisplay display;
	SEIO IO;
	OpenGLShaderManager shadman;
	TextureManager texMan;
	ModelManager modMan;
	World world;
	Input& input;
	Impl():input(Input::Instance()){}
};

Engine::Engine() : pImpl(new Impl()) {}

OpenGLRenderer& Engine::GetRenderer()
{
	return pImpl->display.GetRenderer();
}

GLFWDisplay& Engine::GetDisplay()
{
	return pImpl->display;
}

SEIO& Engine::GetIO() {
	return pImpl->IO;
}

TextureManager& Engine::GetTextureMan() {
	return pImpl->texMan;
}

ModelManager& Engine::GetModelMan() {
	return pImpl->modMan;
}

World& Engine::GetActiveWorld() {
	return pImpl->world;
}

Input& Engine::GetInput() {
	return pImpl->input;
}

void Engine::Init() {
	// initialize all the systems in order
	pImpl->IO.Init();
	glfwSetErrorCallback(&Engine::GLFWErrorCallback);
	glfwInit();
	pImpl->display.Init();
	glfwSetKeyCallback(pImpl->display.GetWindow(), &Input::GLFWKeyCallback);
	glfwSetCursorPosCallback(pImpl->display.GetWindow(), &Input::GLFWMousePosCallback);
	glfwSetMouseButtonCallback(pImpl->display.GetWindow(), &Input::GLFWMouseButtonCallback);
	pImpl->world.LoadWorld("maps/0.lvl");
	// set last time to prevent big jump on first frame
	pImpl->lastTime = glfwGetTime();
}

bool Engine::Update() {
	// update dtime
	pImpl->deltaTime = glfwGetTime() - pImpl->lastTime;
	pImpl->lastTime = glfwGetTime();
	glfwPollEvents();
	pImpl->world.Update();
	pImpl->display.Render();
	pImpl->input.Frame();
	if (pImpl->input.GetKeyDown(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(pImpl->display.GetWindow(), GLFW_TRUE);
	}
	if (pImpl->input.GetKeyDown(GLFW_KEY_G)) {
		pImpl->world.dunGenerator.GenerateDungeon();
	}
	return !glfwWindowShouldClose(pImpl->display.GetWindow());
}

float Engine::GetDeltaTime() const {
	return pImpl->deltaTime;
}

void Engine::GLFWErrorCallback(int errorcode, const char* msg) {
	std::cout << "Something happened in GLFW: " << errorcode << "\n" << msg << "\n";
}

int Engine::GetWindWidth() const {
	return 800;
}

int Engine::GetWindHeight() const {
	return 600;
}

OpenGLShaderManager& Engine::GetShaderMan() {
	return pImpl->shadman;
}

void Engine::GLFWKeyCallback(GLFWwindow* wind, int key, int, int action, int)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(wind, GLFW_TRUE);
	}
}

void Engine::GLFWCursorPosCallback(GLFWwindow*, double x, double y)
{
	Engine::Instance().mouseX = x;
	Engine::Instance().mouseY = y;
}

void Engine::GLFWMouseButtonCallback(GLFWwindow*, int button, int action, int)
{
	//if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
	//	// to get the clicked pos on the near plane in world space
	//	// we have to multiply it with inverses of each matrix used
	//	// to get from wspace to cspace
	//	Camera* cam = Engine::Instance().GetActiveWorld().GetCamera();
	//	glm::vec4 RayClip((2.0f * Engine::Instance().mouseX) / Engine::Instance().GetWindWidth() - 1.0f, 1.0f - (2.0f * Engine::Instance().mouseY) / Engine::Instance().GetWindHeight(), -1.0f, 1.0f);
	//	glm::vec4 RayEye = glm::inverse(cam->GetPMatrix()) * RayClip;
	//	RayEye.z = -1.0f;
	//	RayEye.w = 0.0f;
	//	glm::vec3 RayWorld = glm::normalize(glm::inverse(cam->GetVMatrix()) * RayEye);
	//	float IntersectDistance;
	//	if (glm::intersectRayPlane(cam->GetPos(), RayWorld, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), IntersectDistance))
	//	{
	//		glm::vec3 clickLoc = cam->GetPos() + (RayWorld * IntersectDistance);
	//		// get the player to move towards the nearest node to the click point
	//		Engine::Instance().GetActiveWorld().GetPlayer()->MoveTowards(clickLoc);
	//	}
	//}
}


