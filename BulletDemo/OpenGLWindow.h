#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <gl/GLU.h>
#include "Scene.h"

/**
 * Main window in the game.
 * Has a current scene, and periodically calls its update and render methods.
 * Implemented using SDL.
 */
class OpenGLWindow
{
public:
	/**
	 * Constructor.
	 * Initializes SDL (OpenGL) context.
	 */
	OpenGLWindow(void);
	~OpenGLWindow(void);

	/**
	 * Sets the scene to be updated/rendered.
	 */
	void setScene(Scene * scene);

	/**
	 * Starts the execution.
	 */
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

