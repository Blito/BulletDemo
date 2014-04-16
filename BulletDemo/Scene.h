#pragma once
#include <SDL2/SDL.h>
class Scene
{
public:
	virtual void update(Uint32 elapsedTimeInMillis) = 0;
	virtual void render() = 0;
	virtual bool isQuit() = 0;
};

