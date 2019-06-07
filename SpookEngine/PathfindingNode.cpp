#include "PathfindingNode.h"
#include <algorithm>
#include <random>
PathfindingNode::PathfindingNode(const glm::vec3& initpos) : 
	pos(initpos), parent(nullptr), dist(0)
{}

void PathfindingNode::AddConnection(PathfindingNode* con) {
	if (con) {
		glm::vec3 normalVec = glm::normalize(con->pos - pos);
		connections.insert(std::pair<PathfindingNode*, glm::vec3>(con, normalVec));
	}
}

template<typename TK, typename TV>
std::vector<TK> extract_keys(std::map<TK, TV> const& input_map) {
	std::vector<TK> retval;
	for (auto const& element : input_map) {
		retval.push_back(element.first);
	}
	return retval;
}

void PathfindingNode::DoPath() {
	std::vector<PathfindingNode*> cons = extract_keys<PathfindingNode*, glm::vec3>(connections);
	// update distance and parent of connected nodes if applicable
	for (auto con : cons) {
		int rnd = rand() % 100;
		if (dist + rnd < con->dist) {
			con->dist = dist + rnd; // random weighting value
			con->parent = this;
		}
	}
	if (!visited) {
		// we have been visited now
		visited = true;
		// visit each node in order of distance
		std::sort(cons.begin(), cons.end(), [](PathfindingNode* a, PathfindingNode* b) {
			return a->dist < b->dist;
		});
		// make them do the same (recurse)
		for (auto con : cons) {
			con->DoPath();
			// we need to crash ASAP if there's a parent loop
			// obviously this shouldn't happen, but if it does, player's nodelist
			// will populate infinitely...
			assert(!(con == parent && con->parent == this));
		}
	}
}