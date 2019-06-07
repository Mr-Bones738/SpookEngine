#ifndef _H_WORLD_
#define _H_WORLD_
#include <string>
#include <vector>
#include "BSPMap.h"
#include "Actor.h"
#include "Camera.h"
#include "Renderable.h"
#include "Player.h"
#include "PathfindingNode.h"
#include "DungeonGen.h"
class World {
public:
	friend class Engine;
	Camera* GetCamera();
	Player* GetPlayer();
	std::vector<Renderable*>& getRList();
	std::vector<PathfindingNode*> getNList() const;
	DungeonGen dunGenerator;
private:
	void LoadWorld(const std::string& name);
	void Update();
	std::vector<Actor*> aList;
	std::vector<Renderable*> rList;
	std::vector<PathfindingNode*> nList;
	Camera* mainCam;
	Player* player;
};
#endif