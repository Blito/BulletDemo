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
	void update(Uint32 elapsedTimeInMillis);
	void render();
	bool isQuit() { return quit; }

private:
	void renderCube(float x = 0, float y = 0, float z = 0);

	float angleH, angleV;
	float xCam, yCam, zCam;
	float lx, ly, lz;
	float mov_speed, rot_speed, mouse_sensitivity;
	bool quit;
};

