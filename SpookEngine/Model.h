#ifndef _H_MODEL_
#define _H_MODEL_
#include <string>
#include <glad/glad.h>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "Texture.h"
class Model {
public:
	friend class ModelManager;
	GLuint GetModelVAO() const;
	GLuint GetVNum() const;
	Texture* GetTex();
	void SwitchVBOToFrame(std::string anim, const GLuint& frame);
	float GetScaleFactor() const;
	bool FrameIsValid(const std::string& anim, const GLuint& frame);
	glm::vec3 GetRotateFactor() const;
	GLuint vbo;
private:
	Model(const std::string& name);
	void LoadFromOBJ(const std::string& name);
	void LoadFromMD2(const std::string& name);
	GLuint vao;
	// pair describes animation
	// second describes each vbo that belongs to that animation
	std::map<std::string, std::vector<GLuint>> vbos;
	Texture* tex;
	float scaleFactor = 1.0f;
	// [2:16 AM] spooky: why is he rotated all r*******
	// [2:16 AM] govie: yeah has to be rotated -90 on x
	glm::vec3 rotateFactor;
	std::string name;
	// it was more worth it to use a vector rather than a raw GLfloat ptr
	// plus my old obj code used a vector :)
	// this is temporary, cleared to save memory after loading
	// update: it's a map to a vector of a vector now
	// pair describes an animation
	// second describes the vertices in each frame of that animation
	std::map<std::string, std::vector<std::vector<GLfloat>>> animVerts;
	GLuint vNum;
};
#endif