#include "OpenGLRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLShaderProgram.h"
#include "Engine.h"
struct OpenGLRenderer::Impl {
	GLuint fbo;
	GLuint rbo;
	GLuint renderTexture;
	OpenGLShaderProgram* rendShad;
};
OpenGLRenderer::OpenGLRenderer() : pImpl(new Impl()) {}

void OpenGLRenderer::Init() {
	// set up GL stuff for us
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	// enable backface culling and depth testing, we will enable culling later
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// set clear colour
	glClearColor(0.4f, 0.0f, 0.7f, 1.0f);
	glViewport(0, 0, Engine::Instance().GetWindWidth(), Engine::Instance().GetWindHeight());
	// generate a framebuffer to render to, bind a colour attachment as a texture
	glGenFramebuffers(1, &pImpl->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, pImpl->fbo);
	glGenTextures(1, &pImpl->renderTexture);
	glBindTexture(GL_TEXTURE_2D, pImpl->renderTexture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		Engine::Instance().GetWindWidth(),
		Engine::Instance().GetWindHeight(),
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pImpl->renderTexture, 0);
	glGenRenderbuffers(1, &pImpl->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, pImpl->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Engine::Instance().GetWindWidth(), Engine::Instance().GetWindHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pImpl->rbo);

	// unbind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// we are using the same shader for everything rn (todo: materials)
	pImpl->rendShad = Engine::Instance().GetShaderMan().GetShaderProgram("fullbright");
}

void OpenGLRenderer::Render() {
	// Note: I would rather use UBOs for the matrices
	// but I don't know if I can use GL 4.2 for this assignment (for expl ubo binding)
	// so i'm using 3.3 instead to ensure compatibility.
	// 1. enable dtest and bind the renderbuffer
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, pImpl->fbo);
	// 2. clear the renderbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 3. render renderables
	pImpl->rendShad->Use();
	pImpl->rendShad->SetVPMat(glm::value_ptr(Engine::Instance().GetActiveWorld().GetCamera()->GetVPMatrix()));
	for (DungeonRoom* it : Engine::Instance().GetActiveWorld().dunGenerator.rooms) {
		pImpl->rendShad->SetMMat(glm::value_ptr(it->mmat));
		it->RoomModel->SwitchVBOToFrame("none", 0);
		glBindTexture(GL_TEXTURE_2D, it->RoomModel->GetTex()->GetTextureID());
		glDrawArrays(GL_TRIANGLES, 0, it->RoomModel->GetVNum());
	}
	for (Renderable* it : Engine::Instance().GetActiveWorld().getRList()) {
		pImpl->rendShad->SetMMat(glm::value_ptr(it->GetMMat()));
		// this also binds the VAO for rendering
		it->GetModel()->SwitchVBOToFrame(it->currentAnimation, it->currentAnimFrame);
		glBindTexture(GL_TEXTURE_2D, it->GetModel()->GetTex()->GetTextureID());
		glDrawArrays(GL_TRIANGLES, 0, it->GetModel()->GetVNum());
	}
	// 4. unbind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);
}

int OpenGLRenderer::GetRenderTexture() {
	return pImpl->renderTexture;
}