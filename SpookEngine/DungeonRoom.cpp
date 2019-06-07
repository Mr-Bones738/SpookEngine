#include "DungeonRoom.h"
#include <glm/gtc/matrix_transform.hpp>
DungeonRoom::DungeonRoom(Model* initModel, glm::vec3 initPos) : RoomModel(initModel), pos(initPos) {
	mmat = glm::mat4(1.0f);
	mmat = glm::translate(mmat, pos);
}