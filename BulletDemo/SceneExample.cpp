#include "SceneExample.h"

SceneExample::SceneExample() : angleH(0), angleV(0),
							   xCam(0), yCam(0), zCam(5), 
							   quit(false), 
							   mov_speed(0.005f), rot_speed(0.006f), mouse_sensitivity(2.0f) {
	lx = sin(angleH);
	ly = sin(angleV);
	lz = -cos(angleH);
}

void SceneExample::update() {
	SDL_Event e;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&e)){
		if (e.type == SDL_QUIT)
			quit = true;
		if (e.type == SDL_MOUSEMOTION)
		{
			/* If the mouse is moving to the left */
			if (e.motion.xrel < 0) {
				angleH -= rot_speed * mouse_sensitivity;
			}
			/* If the mouse is moving to the right */
			else if (e.motion.xrel > 0) {
				angleH += rot_speed * mouse_sensitivity;
			}
			/* If the mouse is moving up */
			else if (e.motion.yrel < 0) {
				angleV += rot_speed * mouse_sensitivity;
			}
			/* If the mouse is moving down */
			else if (e.motion.yrel > 0) {
				angleV -= rot_speed * mouse_sensitivity;
			}
			lx = sin(angleH);
			ly = sin(angleV);
			lz = -cos(angleH);
		}
	}
	if (keys[SDL_SCANCODE_W]) {
		xCam += lx * mov_speed;
		zCam += lz * mov_speed;
	}
	if (keys[SDL_SCANCODE_A]) {
		angleH -= rot_speed;
		lx = sin(angleH);
		lz = -cos(angleH);
	}
	if (keys[SDL_SCANCODE_S]) {
		xCam -= lx * mov_speed;
		zCam -= lz * mov_speed;
	}
	if (keys[SDL_SCANCODE_D]) {
		angleH += rot_speed;
		lx = sin(angleH);
		lz = -cos(angleH);
	}
	if (keys[SDL_SCANCODE_Q]) {
		float dx = cos(angleH) * mov_speed;
		float dz = -sin(angleH) * mov_speed;
		xCam -= dx;
		zCam += dz;
	}
	if (keys[SDL_SCANCODE_E]) {
		float dx = cos(angleH) * mov_speed;
		float dz = -sin(angleH) * mov_speed;
		xCam += dx;
		zCam -= dz;
	}
	if (keys[SDL_SCANCODE_UP]) {
		angleV += rot_speed;
		ly = sin(angleV);
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		angleV -= rot_speed;
		ly = sin(angleV);
	}
	if (keys[SDL_SCANCODE_SPACE]) {
		yCam = 1.5;
	}
	if (keys[SDL_SCANCODE_ESCAPE]) {
		quit = true;
	}
	yCam = yCam > 0 ? yCam - 0.005f : 0;
	
}

void SceneExample::render() {
	glLoadIdentity();
	gluLookAt(xCam, yCam, zCam,
			xCam+lx, yCam+ly,  zCam+lz,
			0.0f, 1.0f,  0.0f);
	glScalef(0.5,0.5,0.5);

	renderCube(-3,0,0);
	renderCube(0,0,0);
	renderCube(0,3,0);
	renderCube(3,0,0);

	glFlush();
}

void SceneExample::renderCube(float x, float y, float z) {

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