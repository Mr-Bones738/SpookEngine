#include "Player.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/epsilon.hpp>
#include <limits>
#include <algorithm>
Player::Player(Model* model, PathfindingNode* initNode) : Renderable(initNode->pos, model) {
	lastNode = initNode;
	SetAnimation("stand");
}
Player::Player(Model* model, const glm::vec3& initPos) : Renderable(initPos, model) {
	SetAnimation("stand");
}
void Player::Update() {
	if (!nodesToVisit.empty()) {
		// using 0.2 as epsilon because it may be dumb and keep going past ep range
		if (glm::all(glm::epsilonEqual(pos, nodesToVisit.back()->pos, 0.2f))) {
			lastNode = nodesToVisit.back();
			nodesToVisit.pop_back();
		}
		// if we're still not empty
		if (!nodesToVisit.empty()) {
			// if we have a node to go to and we haven't reached it yet, go towards it
			pos += (glm::normalize(nodesToVisit.back()->pos - pos) * Engine::Instance().GetDeltaTime()) * moveSpeed;
			// glm::lookAt is reversed (because it's for vertices to move in relation to camera)
			// so we have to invert it
			rotMat = glm::inverse(glm::lookAt(pos, nodesToVisit.back()->pos, glm::vec3(0.0f, 1.0f, 0.0f)));
		}
		else {
			SetAnimation("stand");
		}
	}
	UpdateAnimation();
}