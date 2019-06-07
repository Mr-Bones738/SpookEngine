#ifndef _H_OGLSHADMAN_
#define _H_OGLSHADMAN_
#include "OpenGLShaderProgram.h"
#include <string>
#include <vector>
class OpenGLShaderManager {
public:
	OpenGLShaderProgram* GetShaderProgram(const std::string& name);
private:
	std::vector<OpenGLShaderProgram*> loadedProgs;
};

#endif