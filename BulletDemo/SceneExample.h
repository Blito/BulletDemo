#pragma once
#include "Scene.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <gl/GLU.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <math.h>

class SceneExample :
	public Scene
{
public:
	SceneExample(void);
	~SceneExample(void);
	void update(Uint32 elapsedTimeInMillis);
	void render();
	bool isQuit() { return quit; }
	SDL_Texture * getText() const { return text; }

private:
	void renderCube(float x = 0, float y = 0, float z = 0);
	void renderPlane(float y = 0);
	void createText(const std::string &message, SDL_Color color);

	float angleH, angleV;
	float xCam, yCam, zCam;
	float lx, ly, lz;
	float mov_speed, rot_speed, mouse_sensitivity;
	bool quit;
	TTF_Font *font;
	SDL_Texture * text;
};

