#ifndef _H_PLAYER_
#define _H_PLAYER_
#include "Renderable.h"
#include "PathfindingNode.h"
#include <vector>
class Player : public Renderable {
public:
	Player(Model* model, PathfindingNode* initNode);
	Player(Model* model, const glm::vec3& initPos);
	void Update() override;
	void MoveTowards(const glm::vec3& target);
private:
	PathfindingNode* lastNode;
	std::vector<PathfindingNode*> nodesToVisit;
	float moveSpeed = 5.0f;
};
#endif