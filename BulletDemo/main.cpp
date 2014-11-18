#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "OpenGLWindow.h"
#include "SceneExample.h"

int	main(int argc, char *argv[])
{
	LittleLab::Graphics::OpenGLWindow window(800, 600);
	SceneExample scene;

	window.setScene(&scene);

	//Our event structure
	window.run();

	return 0;
}