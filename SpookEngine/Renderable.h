#ifndef _H_RENDERABLE_
#define _H_RENDERABLE_
#include "Actor.h"
#include "Model.h"
#include <string>
class Renderable : public Actor {
public:
	Renderable(const glm::vec3& initPos, Model* model);
	glm::mat4 GetMMat() const;
	Model* GetModel() const;
	void Update() override;
	void SetAnimation(const std::string& animName);
	void UpdateAnimation();
	GLuint currentAnimFrame = 0;
	std::string currentAnimation = "none";
protected:
	glm::mat4 rotMat;
private:
	float clock = 0.0f;
	Model* model;
	bool animated;
};
#endif