#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message too
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg.c_str() << " error: " << SDL_GetError() << std::endl;
}

/**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr)		
		logSDLError(std::cout, "LoadTexture");
	return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param rend The renderer we want to draw too
* @param dst The destination rectangle to render the texture too
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
	SDL_Rect *clip = nullptr)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		//Query the texture to get its width and height to use
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}

	renderTexture(tex, ren, dst, clip);
}

/**
 * Tile the texture to fill the screen. Doesn't wrap to the borders.
 */
void tileTexture(SDL_Texture * texture, SDL_Renderer * ren) {
	int iW, iH;
	SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);

	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			renderTexture(texture, ren, x, y);
			y += iH;
		}
		x += iW;
	}
}

int main(int argc, char** argv)
{
	
	// Initialize everything
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	// Initialize SDL_image for PNG images
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		logSDLError(std::cout, "IMG_Init");
		return 1;
	}

	// Create a window
	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480,
		SDL_WINDOW_SHOWN);
	if (win == nullptr){
		logSDLError(std::cout, "SDL_CreateWindow");
		return 1;
	}

	// Create a renderer
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		logSDLError(std::cout, "SDL_CreateRenderer");
		return 1;
	}

	SDL_Texture * background = loadTexture("resources/background.bmp", ren);
	SDL_Texture * image = loadTexture("resources/image.png", ren);
	if (background == nullptr || image == nullptr)
		return 4;

	//iW and iH are the clip width and height
	//We'll be drawing only clips so get a center position for the w/h of a clip
	int iW = 100, iH = 100;
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	//Setup the clips for our image
	SDL_Rect clips[4];
	for (int i = 0; i < 4; ++i){
		clips[i].x = i / 2 * iW;
		clips[i].y = i % 2 * iH;
		clips[i].w = iW;
		clips[i].h = iH;
	}
	//Specify a default clip to start with
	int useClip = 0;

	//Our event structure
	SDL_Event e;
	bool quit = false;
	while (!quit){
		const Uint8 *keys = SDL_GetKeyboardState(NULL);
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT)
				quit = true;
			if (keys[SDL_SCANCODE_UP]) { // Pressed up key
				y -= 5;
			}
			if (keys[SDL_SCANCODE_DOWN]) { // Pressed down key
				y += 5;
			}
			if (keys[SDL_SCANCODE_LEFT]) {
				x -= 5;
			}
			if (keys[SDL_SCANCODE_RIGHT]) {
				x += 5;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				useClip++;
				useClip %= 4;
			}
			if (keys[SDL_SCANCODE_ESCAPE]) {
				quit = true;
			}
		}

		//Render the scene
		SDL_RenderClear(ren);
		renderTexture(image, ren, x, y, &clips[useClip]);
		SDL_RenderPresent(ren);
	}

	// Clean up
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	SDL_Quit();

	return 0;
}