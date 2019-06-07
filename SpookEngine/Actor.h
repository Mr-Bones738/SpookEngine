#ifndef _H_ACTOR_
#define _H_ACTOR_
#include <string>
#include <glm/glm.hpp>
class Actor {
public:
	virtual void Update() = 0;
	std::string GetName() const;
	glm::vec3 GetPos() const;
protected:
	glm::vec3 pos;
private:
	std::string name;
};
#endif