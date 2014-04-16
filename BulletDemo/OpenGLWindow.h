#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <gl/GLU.h>
#include "Scene.h"

class OpenGLWindow
{
public:
	OpenGLWindow(void);
	~OpenGLWindow(void);
	void setScene(Scene * scene);
	void run();

private:
	void renderFrame();
	void Display_InitGL();
	int Display_SetViewport(int width, int height);

	SDL_Window* displayWindow;
	SDL_Renderer* displayRenderer;
	Scene * activeScene;
	Uint32 elapsedTime;
};

