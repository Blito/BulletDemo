#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <gl/GLU.h>

SDL_Window* displayWindow;
SDL_Renderer* displayRenderer;
GLUquadricObj* quad;

float angle = 0, xCam = 0, yCam = 0, zCam = 5, lx = sin(angle), lz = -cos(angle), mov_speed = 0.005, rot_velocity = 0.006;
													   
void Display_InitGL()
{
	/* Enable smooth shading */
	glShadeModel( GL_SMOOTH );

	/* Set the background black */
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	/* Depth buffer setup */
	glClearDepth( 1.0f );

	/* Enables Depth Testing */
	glEnable( GL_DEPTH_TEST );

	/* The Type Of Depth Test To Do */
	glDepthFunc( GL_LEQUAL );

	/* Really Nice Perspective Calculations */
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	quad = gluNewQuadric();
}
/* function to reset our viewport after a window resize */
int Display_SetViewport( int width, int height )
{
	/* Height / width ration */
	GLfloat ratio;

	/* Protect against a divide by zero */
	if ( height == 0 ) {
		height = 1;
	}

	ratio = ( GLfloat )width / ( GLfloat )height;

	/* Setup our viewport. */
	glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

	/* change to the projection matrix and set our viewing volume. */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );

	/* Set our perspective */
	gluPerspective( 45.0f, ratio, 0.1f, 100.0f );

	/* Make sure we're chaning the model view and not the projection */
	glMatrixMode( GL_MODELVIEW );

	/* Reset The View */
	glLoadIdentity( );

	return 1;
}

void renderCube(float x = 0, float y = 0, float z = 0) {

	glPushMatrix();
	glTranslatef(x, y, z);

	glBegin(GL_QUADS);

	/* Cube Top */
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);


	/* Cube Bottom */
	glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	/* Cube Front */
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	/* Cube Back */
	glColor4f(0.0f, 1.0f, 0.5f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	/* Cube Left Side */
	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	/* Cube Right Side */
	glColor4f(0.15f, 0.25f, 0.75f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);


	glEnd();

	glPopMatrix();
}

void renderScene() {
	glLoadIdentity();
	gluLookAt(xCam, yCam, zCam,
			xCam+lx, yCam,  zCam+lz,
			0.0f, 1.0f,  0.0f);
	glScalef(0.5,0.5,0.5);

	renderCube(-3,0,0);
	renderCube(0,0,0);
	renderCube(0,3,0);
	renderCube(3,0,0);

	glFlush();
}

void Display_Render()
{
	/* Set the background black */
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	/* Clear The Screen And The Depth Buffer */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	renderScene();

	SDL_RenderPresent(displayRenderer);
	SDL_GL_SwapWindow(displayWindow);
}


int	main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_RendererInfo displayRendererInfo;
	SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
	SDL_GLContext glContext = SDL_GL_CreateContext(displayWindow);
	SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
	/*TODO: Check that we have OpenGL */
	if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 || 
		(displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
			/*TODO: Handle this. We have no render surface and not accelerated. */
	}

	Display_InitGL();

	Display_SetViewport(800, 600);

	//Our event structure
	SDL_Event e;
	bool quit = false;
	while (!quit) {
		const Uint8 *keys = SDL_GetKeyboardState(NULL);
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT)
				quit = true;
		}
		if (keys[SDL_SCANCODE_W]) {
			xCam += lx * mov_speed;
			zCam += lz * mov_speed;
		}
		if (keys[SDL_SCANCODE_A]) {
			angle -= rot_velocity;
			lx = sin(angle);
			lz = -cos(angle);
		}
		if (keys[SDL_SCANCODE_S]) {
			xCam -= lx * mov_speed;
			zCam -= lz * mov_speed;
		}
		if (keys[SDL_SCANCODE_D]) {
			angle += rot_velocity;
			lx = sin(angle);
			lz = -cos(angle);
		}
		if (keys[SDL_SCANCODE_Q]) {
			float dx = cos(angle) * mov_speed;
			float dz = -sin(angle) * mov_speed;
			xCam -= dx;
			zCam += dz;
		}
		if (keys[SDL_SCANCODE_E]) {
			float dx = cos(angle) * mov_speed;
			float dz = -sin(angle) * mov_speed;
			xCam += dx;
			zCam -= dz;
		}
		if (keys[SDL_SCANCODE_SPACE]) {
			yCam = 1.5;
		}
		if (keys[SDL_SCANCODE_ESCAPE]) {
			quit = true;
		}
		yCam = yCam > 0 ? yCam - 0.005 : 0;
		Display_Render();
	}

	SDL_Quit();

	return 0;
}