#ifndef _H_OGLSHADPROG_
#define _H_OGLSHADPROG_
#include <string>
#include <glad/glad.h>
class OpenGLShaderProgram {
public:
	void Use();
	GLuint GetUniformLoc(const std::string& name) const;
	void SetVPMat(const GLfloat* value);
	void SetMMat(const GLfloat* value);
	friend class OpenGLShaderManager;
private:
	// I only want the shader manager to be able to construct these
	OpenGLShaderProgram(const std::string& shadname);
	GLuint shad;
	GLuint vpLoc;
	GLuint mLoc;
	std::string name;
};

#endif