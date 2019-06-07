#include "GLFWDisplay.h"
#include "Engine.h"

// this class takes the texture from an OpenGLRenderer and displays it on a
// quad that takes up the entire screen.
// this is so that the renderer class itself is decoupled from GLFW
// (so i can use it elsewhere...)

struct GLFWDisplay::Impl {
	OpenGLRenderer rend;
	GLFWwindow* wind;
	GLuint quadVbo;
	GLuint quadVao;
	// kole's note: spent 2 hours debugging this because i thought vertex order was meant to be clockwise
	GLfloat quadVerts[30]{
		 1.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom right
		-1.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top left
		-1.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom left
		-1.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top left
		 1.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom right
		 1.0f,  1.0f,  0.0f, 1.0f, 1.0f  // top right
	};
	OpenGLShaderProgram* quadProg;
	Impl() :
		wind(nullptr),
		quadVbo(0),
		quadVao(0),
		quadProg(nullptr)
	{}
};

GLFWDisplay::GLFWDisplay() : pImpl(new Impl) {}

void GLFWDisplay::Init() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // for macOS compatibility, even tho IO code makes this an impossiblity rn
	pImpl->wind = glfwCreateWindow(Engine::Instance().GetWindWidth(), 
								   Engine::Instance().GetWindHeight(), 
								   "dont crash", nullptr, nullptr);
	glfwSetInputMode(pImpl->wind, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(pImpl->wind);
	pImpl->rend.Init();
	pImpl->quadProg = Engine::Instance().GetShaderMan().GetShaderProgram("flat");
	pImpl->quadProg->Use();
	// set up the vao for the screenquad
	glGenVertexArrays(1, &pImpl->quadVao);
	glGenBuffers(1, &pImpl->quadVbo);
	glBindVertexArray(pImpl->quadVao);
	glBindBuffer(GL_ARRAY_BUFFER, pImpl->quadVbo);
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(GLfloat), pImpl->quadVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void GLFWDisplay::Render() {
	// tell the render to render to its texture
	glBindVertexArray(0);
	pImpl->rend.Render();
	// disable depth testing (this is faster than clearing the depth buffer)
	glDisable(GL_DEPTH_TEST);
	// get the rendered texture from the renderer
	glBindTexture(GL_TEXTURE_2D, pImpl->rend.GetRenderTexture());
	// use the simple shader program (flat.vs/fs)
	pImpl->quadProg->Use();
	// bind the quad's vao and draw it
	glBindVertexArray(pImpl->quadVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// push to frontbuffer
	glfwSwapBuffers(pImpl->wind);
}

OpenGLRenderer& GLFWDisplay::GetRenderer() {
	return pImpl->rend;
}

GLFWwindow* GLFWDisplay::GetWindow() {
	return pImpl->wind;
}