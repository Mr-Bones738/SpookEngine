#include "DungeonGen.h"
#include <random>
#include <time.h>
#include <limits>
#include "Engine.h"
#include <iostream>
void CreateLink(PathfindingNode* a, PathfindingNode* b) {
	a->AddConnection(b);
	b->AddConnection(a);
}
void DungeonGen::GenerateDungeon() {
	rooms.clear();
	float begin = glfwGetTime();
	std::vector<PathfindingNode*> nodeList;
	int roomSize = 20; // changeme depending on actual room size
	int widthInRooms = 100;
	int breadthInRooms = 100;
	glm::vec3 room1pos(0);
	// generate a bunch of nodes with positions
	for (int z = 0; z < breadthInRooms; z++) {
		glm::vec3 thisPos(0);
		for (int x = 0; x < widthInRooms; x++) {
			thisPos.x = room1pos.x + (x * roomSize);
			thisPos.z = room1pos.z + (z * roomSize);
			nodeList.push_back(new PathfindingNode(thisPos));
		}
	}
	// link up each node to all adjacent nodes
	for (int i = 0; i < (widthInRooms * breadthInRooms) - widthInRooms; i++) {
		CreateLink(nodeList[i], nodeList[i + widthInRooms]);
	}
	for (int y = 0; y < (widthInRooms * breadthInRooms); y += breadthInRooms) {
		for (int x = 0; x < widthInRooms - 1; x++) {
			CreateLink(nodeList[y + x], nodeList[(y + 1) + x]);
		}
	}
	// set default values for nodes
	for (auto it : nodeList) {
		it->parent = nullptr;
		it->active = false;
		if (it == nodeList[0]) it->dist = 0;
		else it->dist = std::numeric_limits<int>::max();
	}
	srand(time(nullptr)); // seed random with time
	// do a pathfind from node 0 to node 24 (bleft to tright)
	nodeList[0]->DoPath();

	// go through each parent and add them to a list while marking them as active (this is for door selection)
	PathfindingNode* par = nodeList[(breadthInRooms * widthInRooms) - 1];
	std::vector<PathfindingNode*> eList;
	while (par != nullptr) {
		par->active = true;
		eList.push_back(par);
		par = par->parent;
	}
	// for added flavour, randomly activate inactive connections to rooms
	std::vector<PathfindingNode*> actList;
	for (auto it : eList) {
		// 50% chance
		if (rand() % 100 < 50) {
			// if there's an inactive connection, activate it
			for (auto n : it->connections) {
				if (!n.first->active) {
					n.first->active = true;
					actList.push_back(n.first);
					break;
				}
				// 10% chance to branch again
				if (rand() % 100 < 10) {
					for (auto n2 : n.first->connections) {
						if (!n2.first->active) {
							n2.first->active = true;
							actList.push_back(n.first);
							break;
						}
					}
				}
			}
		}
	}
	for (auto it : actList) {
		eList.push_back(it);
	}
	for (auto it : eList) {
		// flags to keep track of which directions are connected to this and active
		unsigned char dirFlags = 0; // 0000NESW
		for (auto c : it->connections) {
			if (c.first->active) {
				if (c.second.x == -1.0f)
					dirFlags = dirFlags | 0b00000001;
				if (c.second.z == 1.0f)
					dirFlags = dirFlags | 0b00000010;
				if (c.second.x == 1.0f)
					dirFlags = dirFlags | 0b00000100;
				if (c.second.z == -1.0f)
					dirFlags = dirFlags | 0b00001000;
			}
		}
		// todo: replace room meshes with room descriptors that use tiles
		Model* dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/room.obj");
		switch (dirFlags) {
		case 0x1: // WEST
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomw.obj");
			break;
		case 0x2: // SOUTH
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/rooms.obj");
			break;
		case 0x3: // SOUTH + WEST
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomsw.obj");
			break;
		case 0x4: // EAST
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roome.obj");
			break;
		case 0x5: // EAST + WEST
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomew.obj");
			break;
		case 0x6: // EAST + SOUTH
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomes.obj");
			break;
		case 0x7: // EAST + SOUTH + WEST
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomesw.obj");
			break;
		case 0x8: // NORTH
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomn.obj");
			break;
		case 0x9: // NORTH + WEST
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomnw.obj");
			break;
		case 0xA: // NORTH + SOUTH
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomns.obj");
			break;
		case 0xB: // NORTH + SOUTH + WEST
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomnsw.obj");
			break;
		case 0xC: // NORTH + EAST
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomne.obj");
			break;
		case 0xD: // NORTH + EAST + WEST
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomnew.obj");
			break;
		case 0xE: // NORTH + EAST + SOUTH
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomnes.obj");
			break;
		case 0xF: // NORTH + EAST + SOUTH + WEST
			dModel = Engine::Instance().GetModelMan().GetModel("tileset/test/roomnesw.obj");
			break;
		default:
			std::cout << "dungen: what the fuck?\n";
			break;
		}
		rooms.push_back(new DungeonRoom(dModel, it->pos));
	}
	std::cout << "Time to generate: " << glfwGetTime() - begin << "\n";
}