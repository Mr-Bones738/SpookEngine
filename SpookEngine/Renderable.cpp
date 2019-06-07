#include "Renderable.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
Renderable::Renderable(const glm::vec3& initPos, Model* model) {
	this->model = model;
	this->pos = initPos;
}

glm::mat4 Renderable::GetMMat() const {
	// TODO: Only regenerate MMAT when it is required, make this return a pre-generated mat
	glm::mat4 mmat = glm::mat4(1.0f);
	float scaleFactor = model->GetScaleFactor();
	glm::vec3 rotation = model->GetRotateFactor();
	mmat = glm::translate(mmat, pos);
	// combining rotation matrices together doesn't seem to work so we:
	// 1. turn the main rotation into a quaternion
	glm::quat q(rotMat);
	// 2. create a temporary mat for the second rotation
	glm::mat4 r2(1.0f);
	// 3. rotate it
	r2 = glm::rotate(r2, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	r2 = glm::rotate(r2, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	r2 = glm::rotate(r2, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	// 4. combine the rots
	q *= glm::quat(r2);
	// 5. turn it back into a mat and add it to the model matrix
	mmat *= glm::mat4(q);
	mmat = glm::scale(mmat, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	return mmat;
}

Model* Renderable::GetModel() const {
	return model;
}
void Renderable::SetAnimation(const std::string& animName) {
	currentAnimFrame = 0;
	currentAnimation = animName;
}
void Renderable::UpdateAnimation() {
	if (currentAnimation != "none") {
		clock += Engine::Instance().GetDeltaTime();
		// we are running at a maximum of 25fps for animation
		if (clock > 0.04f) {
			// advance frame
			//int framesToAdvance = (int)roundf(clock / 0.04f);
			int targetFrame = currentAnimFrame + 1;
			if (model->FrameIsValid(currentAnimation, targetFrame)) {
				currentAnimFrame = targetFrame;
			}
			else {
				currentAnimFrame = 0;
			}
			clock = 0.0f;
		}
	}
}
void Renderable::Update() {
}