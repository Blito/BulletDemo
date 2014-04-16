#include "SceneExample.h"

#define ANGLEH 0
#define ANGLEV 0
#define XCAM 0
#define YCAM 0
#define ZCAM 5
#define MOV_SPEED 0.0045f
#define ROT_SPEED 0.004f
#define MOUSE_SENSITIVITY 0.8f

SceneExample::SceneExample() : angleH(ANGLEH), angleV(ANGLEV),
							   xCam(XCAM), yCam(YCAM), zCam(ZCAM), 
							   quit(false), 
							   mov_speed(MOV_SPEED), rot_speed(ROT_SPEED), mouse_sensitivity(MOUSE_SENSITIVITY) {
	lx = sin(angleH);
	ly = sin(angleV);
	lz = -cos(angleH);
}

void SceneExample::update(Uint32 elapsedTimeInMillis) {
	SDL_Event e;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&e)){
		if (e.type == SDL_QUIT)
			quit = true;
		if (e.type == SDL_MOUSEMOTION)
		{
			/* If the mouse is moving to the left */
			if (e.motion.xrel < 0) {
				angleH -= rot_speed * -e.motion.xrel * mouse_sensitivity * elapsedTimeInMillis;
			}
			/* If the mouse is moving to the right */
			else if (e.motion.xrel > 0) {
				angleH += rot_speed * e.motion.xrel * mouse_sensitivity * elapsedTimeInMillis;
			}
			/* If the mouse is moving up */
			if (e.motion.yrel < 0) {
				if (angleV < 0.5)
					angleV += rot_speed * -e.motion.yrel * mouse_sensitivity * elapsedTimeInMillis;
				else
					angleV = 0.5f;
			}
			/* If the mouse is moving down */
			else if (e.motion.yrel > 0) {
				if (angleV > -0.5)
					angleV -= rot_speed * e.motion.yrel * mouse_sensitivity * elapsedTimeInMillis;
				else
					angleV = -0.5f;
			}
			lx = sin(angleH);
			ly = sin(angleV);
			lz = -cos(angleH);
		}
	}
	if (keys[SDL_SCANCODE_W]) {
		xCam += lx * mov_speed * elapsedTimeInMillis;
		zCam += lz * mov_speed * elapsedTimeInMillis;
	}
	if (keys[SDL_SCANCODE_A]) {
		angleH -= rot_speed * elapsedTimeInMillis;
		lx = sin(angleH);
		lz = -cos(angleH);
	}
	if (keys[SDL_SCANCODE_S]) {
		xCam -= lx * mov_speed * elapsedTimeInMillis;
		zCam -= lz * mov_speed * elapsedTimeInMillis;
	}
	if (keys[SDL_SCANCODE_D]) {
		angleH += rot_speed * elapsedTimeInMillis;
		lx = sin(angleH);
		lz = -cos(angleH);
	}
	if (keys[SDL_SCANCODE_Q]) {
		xCam -= cos(angleH) * mov_speed * elapsedTimeInMillis;
		zCam += -sin(angleH) * mov_speed * elapsedTimeInMillis;
	}
	if (keys[SDL_SCANCODE_E]) {
		xCam += cos(angleH) * mov_speed * elapsedTimeInMillis;
		zCam -= -sin(angleH) * mov_speed * elapsedTimeInMillis;
	}
	if (keys[SDL_SCANCODE_UP]) {
		angleV += rot_speed * elapsedTimeInMillis;
		ly = sin(angleV);
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		angleV -= rot_speed * elapsedTimeInMillis;
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
	renderPlane(-2);

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

void SceneExample::renderPlane(float y) {
	glPushMatrix();

	glTranslatef(0,y,0);
	glScalef(100, 0, 100);

	glBegin(GL_QUADS);

	glColor3f(0.7, 0.7, 0.7);
	glVertex3f(-1.0f, y, 1.0f);
	glVertex3f(-1.0f, y, -1.0f);
	glVertex3f(1.0f, y, -1.0f);
	glVertex3f(1.0f, y, 1.0f);

	glEnd();

	glPopMatrix();
}