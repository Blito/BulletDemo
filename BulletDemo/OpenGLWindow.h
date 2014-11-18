#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <gl/GLU.h>
#include "Scene.h"

namespace LittleLab {

	namespace Graphics {

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
			OpenGLWindow(int width, int height);
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

	}

}