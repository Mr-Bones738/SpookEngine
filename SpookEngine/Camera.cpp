#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine.h"
// TODO: Only calculate VMAT when it is required to be refreshed (i.e. when player moves)
Camera::Camera(Actor* initTarget) : target(initTarget)
{
	if (initTarget != nullptr) {
		pos = target->GetPos();
		pos += glm::vec3(0.0f, 25.0f, 15.0f); // so we're always looking down at target
		view = glm::lookAt(pos, target->GetPos(), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		pos = glm::vec3(0.0f, 0.0f, 0.0f);
		fwd = glm::vec3(0.0f, 0.0f, -1.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		right = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	int w = Engine::Instance().GetWindWidth();
	int h = Engine::Instance().GetWindHeight();
	proj = glm::perspective(45.0f, (float)w / (float)h, 0.1f, 10000.0f);
}

void Camera::Update() {
	if (target != nullptr) {
		pos = target->GetPos();
		pos += glm::vec3(0.0f, 25.0f, 15.0f); // so we're always looking down at target
		// update the view matrix
		view = glm::lookAt(pos, target->GetPos(), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		float dTime = Engine::Instance().GetDeltaTime();
		if (Engine::Instance().GetInput().GetKey(GLFW_KEY_W)) {
			pos += (fwd * dTime) * MoveSpeed;
		}
		if (Engine::Instance().GetInput().GetKey(GLFW_KEY_S)) {
			pos -= (fwd * dTime) * MoveSpeed;
		}
		if (Engine::Instance().GetInput().GetKey(GLFW_KEY_D)) {
			pos += (right * dTime) * MoveSpeed;
		}
		if (Engine::Instance().GetInput().GetKey(GLFW_KEY_A)) {
			pos -= (right * dTime) * MoveSpeed;
		}
		if (Engine::Instance().GetInput().GetKey(GLFW_KEY_Z)) {
			pos += (up * dTime) * MoveSpeed;
		}
		if (Engine::Instance().GetInput().GetKey(GLFW_KEY_X)) {
			pos += (-up * dTime) * MoveSpeed;
		}
		glm::dvec2 dm = Engine::Instance().GetInput().GetMouseDelta() * sensitivity;
		yaw += dm.x;
		pitch -= dm.y;
		if (yaw > 360.0f) {
			yaw = yaw - 360.0f;
		}
		if (yaw < -360.0f) {
			yaw = yaw + 360.0f;
		}
		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}
		fwd.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		fwd.y = sin(glm::radians(pitch));
		fwd.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		fwd = glm::normalize(fwd);
		right = glm::normalize(glm::cross(fwd, up));
		view = glm::lookAt(pos, pos + fwd, up);
	}
}

glm::mat4 Camera::GetVPMatrix() const {
	return proj * view;
}
glm::mat4 Camera::GetVMatrix() const {
	return view;
}
glm::mat4 Camera::GetPMatrix() const {
	return proj;
}