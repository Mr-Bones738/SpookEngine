#ifndef _H_CAMERA_
#define _H_CAMERA_
#include "Actor.h"
#include <glm/glm.hpp>
class Camera : public Actor {
public:
	Camera(Actor* initTarget);
	void Update() override;
	glm::mat4 GetVPMatrix() const;
	glm::mat4 GetVMatrix() const;
	glm::mat4 GetPMatrix() const;
private:
	Actor* target;
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 fwd;
	glm::vec3 up;
	glm::vec3 right;
	float yaw;
	float pitch;
	float MoveSpeed = 100.0f;
	double sensitivity = 0.5;
};

#endif