#ifndef _H_PFNODE_
#define _H_PFNODE_
#include <glm/glm.hpp>
#include <vector>
#include <map>
class PathfindingNode {
public:
	PathfindingNode(const glm::vec3& initpos);
	void AddConnection(PathfindingNode* con);
	void DoPath();
	glm::vec3 pos;
	bool active = false;
	std::map<PathfindingNode*, glm::vec3> connections;
	PathfindingNode* parent;
	int dist;
	bool visited = false;
};
#endif