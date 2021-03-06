#include "OpenGLWindow.h"
#include <stdio.h>
#include <iostream>
#include <cassert>

namespace LittleLab {

	namespace Graphics {

		OpenGLWindow::OpenGLWindow(int width, int height)
		{
			SDL_Init(SDL_INIT_VIDEO);

			// Ask for OpenGL 3.2 context
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

			SDL_RendererInfo displayRendererInfo;
			SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
			SDL_GLContext glContext = SDL_GL_CreateContext(displayWindow);
			SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);

			// Check that we have OpenGL
			glewExperimental = GL_TRUE;
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				fprintf(stdout, "Error: %s\n", glewGetErrorString(err));
			}
			fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

			if (glewIsSupported("GL_VERSION_3_2")) {
				fprintf(stdout, "OpenGl 3.2 supported.\n");
			}

			if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
				(displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
				/*TODO: Handle this. We have no render surface and not accelerated. */
			}

			while ((err = glGetError()) != GL_NO_ERROR) {
				std::cerr << err;
			}

			Display_InitGL();

			while ((err = glGetError()) != GL_NO_ERROR) {
				std::cerr << err;
			}

			Display_SetViewport(width, height);

			while ((err = glGetError()) != GL_NO_ERROR) {
				std::cerr << err;
			}

		}

		OpenGLWindow::~OpenGLWindow(void)
		{
			SDL_Quit();
		}

		void OpenGLWindow::setScene(Scene * scene) {
			activeScene = scene;
			activeScene->setRenderer(displayRenderer);
		}

		void OpenGLWindow::run() {
			elapsedTime = SDL_GetTicks();
			while (!activeScene->isQuit()) {
				renderFrame();
			}
		}

		void OpenGLWindow::renderFrame() {
			/* Set the background black */
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			/* Clear The Screen And The Depth Buffer */
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			activeScene->update(SDL_GetTicks() - elapsedTime);
			elapsedTime = SDL_GetTicks();
			activeScene->render();

			SDL_RenderPresent(displayRenderer);
			SDL_GL_SwapWindow(displayWindow);
		}

		void OpenGLWindow::Display_InitGL()
		{
			/* Enable smooth shading */
			glShadeModel(GL_SMOOTH);

			/* Set the background black */
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

			/* Depth buffer setup */
			glClearDepth(1.0f);

			/* Enables Depth Testing */
			glEnable(GL_DEPTH_TEST);

			/* The Type Of Depth Test To Do */
			glDepthFunc(GL_LEQUAL);

			/* Really Nice Perspective Calculations */
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		}

		/* function to reset our viewport after a window resize */
		int OpenGLWindow::Display_SetViewport(int width, int height)
		{
			/* Height / width ration */
			GLfloat ratio;

			/* Protect against a divide by zero */
			if (height == 0) {
				height = 1;
			}

			ratio = (GLfloat)width / (GLfloat)height;

			/* Setup our viewport. */
			glViewport(0, 0, (GLsizei)width, (GLsizei)height);

			/* change to the projection matrix and set our viewing volume. */
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			/* Set our perspective */
			gluPerspective(45.0f, ratio, 0.1f, 100.0f);

			/* Make sure we're chaning the model view and not the projection */
			glMatrixMode(GL_MODELVIEW);

			/* Reset The View */
			glLoadIdentity();

			return 1;
		}

	}

}