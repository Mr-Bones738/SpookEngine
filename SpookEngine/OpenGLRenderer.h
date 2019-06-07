#ifndef _H_OPENGLRENDERER_
#define _H_OPENGLRENDERER_
class OpenGLRenderer {
public:
	OpenGLRenderer();
	void Init();
	void Render();
	int GetRenderTexture();
private:
	struct Impl;
	Impl* pImpl;
};
#endif