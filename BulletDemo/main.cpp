#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "OpenGLWindow.h"
#include "SceneExample.h"

int	main(int argc, char *argv[])
{

	LittleLab::Graphics::OpenGLWindow window(800, 600);

	LittleLab::Graphics::RenderMgr * renderMgr = new LittleLab::Graphics::RenderMgr();
	LittleLab::Graphics::ShaderMgr * shaderMgr = new LittleLab::Graphics::ShaderMgr();
	LittleLab::Physics::PhysicsMgr * physicsMgr = new LittleLab::Physics::PhysicsMgr();

	SceneExample scene;

	window.setScene(&scene);

	//Our event structure
	window.run();

	delete physicsMgr;
	delete shaderMgr;
	delete renderMgr;

	return 0;
}