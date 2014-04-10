#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char** argv)
{
	
	// Initialize everything
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Create a window
	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480,
		SDL_WINDOW_SHOWN);
	if (win == nullptr){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Create a renderer
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Load the BMP into a Surface
	SDL_Surface *bmp = SDL_LoadBMP("resources/hello.bmp");
	if (bmp == nullptr){
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Create Texture from Surface
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == nullptr){
		std::cout << "SDL_CreateTextureFromSurface Error: "
			<< SDL_GetError() << std::endl;
		return 1;
	}

	// Render
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_RenderPresent(ren);

	// Wait
	SDL_Delay(2000);

	// Clean up
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	SDL_Quit();

	return 0;
}

