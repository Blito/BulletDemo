#pragma once
#include "Scene.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <gl/GLU.h>
#include <math.h>

class SceneExample :
	public Scene
{
public:
	SceneExample(void);
	~SceneExample(void) {}
	void update();
	void render();
	bool isQuit() { return quit; }

private:
	void renderCube(float x = 0, float y = 0, float z = 0);

	float angle;
	float xCam, yCam, zCam;
	float lx, lz;
	float mov_speed, rot_velocity;
	bool quit;
};

