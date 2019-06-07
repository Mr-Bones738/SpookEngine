#ifndef _H_DUNGEN_
#define _H_DUNGEN_
#include "DungeonRoom.h"
#include "PathfindingNode.h"
#include <vector>
class DungeonGen {
public:
	void GenerateDungeon();
	std::vector<DungeonRoom*> rooms;
};
#endif