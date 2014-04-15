#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "OpenGLWindow.h"
#include "SceneExample.h"

int	main(int argc, char *argv[])
{
	OpenGLWindow window;
	SceneExample scene;

	window.setScene(&scene);

	//Our event structure
	while (!scene.isQuit()) {
		window.renderFrame();
	}

	return 0;
}