#include <SDL2/SDL.h>
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
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	//Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;
	//Load the image
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	//If the loading went ok, convert to texture and return the texture
	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		//Make sure converting went ok too
		if (texture == nullptr)
			logSDLError(std::cout, "CreateTextureFromSurface");
	}
	else
		logSDLError(std::cout, "LoadBMP");

	return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
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
	SDL_Texture * image = loadTexture("resources/image.bmp", ren);
	if (background == nullptr || image == nullptr)
		return 4;

	// Render
	SDL_RenderClear(ren);

	// Tiled background
	/*renderTexture(background, ren, 0, 0);
	renderTexture(background, ren, 0, SCREEN_HEIGHT/2);
	renderTexture(background, ren, SCREEN_WIDTH/2, 0);
	renderTexture(background, ren, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);*/
	tileTexture(background, ren);

	// Image
	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	renderTexture(image, ren, SCREEN_WIDTH/2 - iW/2, SCREEN_HEIGHT/2 - iH/2);

	SDL_RenderPresent(ren);

	// Wait
	SDL_Delay(2000);

	// Clean up
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	SDL_Quit();

	return 0;
}

