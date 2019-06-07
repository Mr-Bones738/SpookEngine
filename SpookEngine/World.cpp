#include "World.h"
#include "Engine.h"
void World::LoadWorld(const std::string& name) {
	ModelManager& mman = Engine::Instance().GetModelMan();
	//Renderable* a = new Renderable(glm::vec3(0.0f, 0.0f, 0.0f), mman.GetModel("models/level/0.obj"));
	//aList.push_back(a);
	//rList.push_back(a);

	dunGenerator.GenerateDungeon();
	mainCam = new Camera(nullptr);
}

void World::Update() {
	// tick each entity in the world
	for (Actor* a : aList) {
		a->Update();
	}
	// get camera to update its own position
	mainCam->Update();
}

Player* World::GetPlayer() {
	return player;
}

std::vector<Renderable*>& World::getRList() {
	return rList;
}
std::vector<PathfindingNode*> World::getNList() const {
	return nList;
}
Camera* World::GetCamera() {
	return mainCam;
}