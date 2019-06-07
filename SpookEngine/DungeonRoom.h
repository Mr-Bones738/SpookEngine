#ifndef _H_DUNROOM_
#define _H_DUNROOM_
#include "Model.h"
class DungeonRoom {
public:
	DungeonRoom(Model* initModel, glm::vec3 initPos);
	Model* RoomModel;
	glm::vec3 pos;
	glm::mat4 mmat;
};
#endif