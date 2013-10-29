#include <vld.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "game.h"

using namespace std;

void logSDLError(ostream &os, const char* msg);
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
SDL_Surface* loadMessage(const std::string &msg, const std::string &font, int size, SDL_Colour colour);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr);

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
// Square tiles
const int TILE_SIZE = 40;

int main(int argc, char **argv)
{
	CGame& game = CGame::GetInstance();

	if (!game.Initialise())
	{
		// Failed to initialise.
		return 1;
	}

	while (game.IsActive())
	{
		game.ExecuteOneFrame();
	}

	game.DestroyInstance();

	/*

	if (TTF_Init() != 0)
	{
		logSDLError(cout, "TTF_Init");
		return 3;
	}

	// iW and iH are the clip width and height
	// We'll be drawing only clips so get a center position for the w/h of a clip
	int iW = 100, iH = 100;
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	// Setup the clips for our image
	SDL_Rect clips[4];
	for (int i = 0; i < 4; ++i)
	{
		clips[i].x = i / 2 * iW;
		clips[i].y = i % 2 * iH;
		clips[i].w = iW;
		clips[i].h = iH;
	}
	// Specify a default clip to start with
	int useClip = 0;

	SDL_Texture* image = loadTexture("data/textures/spritesheet.png", renderer);
	if (nullptr == image)
	{
		logSDLError(cout, "loadTexture");
		return 6;
	}

	SDL_Colour col;
	col.r = 255;
	col.g = 255;
	col.b = 255;
	col.a = 0;

	Papyrus::Timer::CTimer* timer = new Papyrus::Timer::CTimer();
	timer->Initialise();
	timer->Start();


		Int8 buffer[MAX_BUFFER];
		timer->Restart();
		sprintf_s(buffer, MAX_BUFFER, "%i", timer->GetFPS());
		SDL_Surface* font = loadMessage(buffer, "data/fonts/SourceSansPro-Regular.ttf", 32, col);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, font);

		renderTexture(image, renderer, x, y, &clips[useClip]);
		renderTexture(message, renderer, 50, 50);

		
		SDL_DestroyTexture(message);
	}

	TTF_Quit();*/

	return 0;
}

/*
*	Log an SDL error with some error message to the output stream of our choice
*	@param os The output stream to write the message too
*	@param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(ostream &os, const char* msg)
{
	os << msg << " error: " << SDL_GetError() << endl;
}

/*
*	Loads an image into a texture on the rendering device
*	@param file The image file to load
*	@param ren The renderer to load the texture onto
*	@return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	SDL_Texture* texture = IMG_LoadTexture(ren, file.c_str());
	if (nullptr == texture)
	{
		logSDLError(cout, "LoadTexture");
	}
	return texture;
}

/*
*	Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
*	the texture's width and height
*	@param tex The source texture we want to draw
*	@param ren The renderer we want to draw too
*	@param x The x coordinate to draw too
*	@param y The y coordinate to draw too
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, SDL_Rect* clip = nullptr)
{
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (nullptr != clip)
	{
		dst.w = clip->w;
		dst.h = clip->h;	
	}
	else 
	{
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}

/*
*	 Draw an SDL_Texture to an SDL_Renderer at some destination rect
*	 taking a clip of the texture if desired
*	 @param tex The source texture we want to draw
*	 @param rend The renderer we want to draw too
*	 @param dst The destination rectangle to render the texture too
*	 @param clip The sub-section of the texture to draw (clipping rect)
*			default of nullptr draws the entire texture
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
}

/*
*	 Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
*	 the texture's width and height and taking a clip of the texture if desired
*	 If a clip is passed, the clip's width and height will be used instead of the texture's
*	 @param tex The source texture we want to draw
*	 @param rend The renderer we want to draw too
*	 @param x The x coordinate to draw too
*	 @param y The y coordinate to draw too
*	 @param clip The sub-section of the texture to draw (clipping rect)
*			default of nullptr draws the entire texture
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

	renderTexture(tex, ren, dst, clip);
}

SDL_Surface* loadMessage(const std::string &msg, const std::string &font, int size, SDL_Colour colour)
{
	TTF_Font* ttfFont = TTF_OpenFont(font.c_str(), size);
	if (nullptr == ttfFont)
	{
		logSDLError(cout, "TTF_OpenFont");
	}

	SDL_Surface* surface = TTF_RenderText_Blended(ttfFont, msg.c_str(), colour);
	TTF_CloseFont(ttfFont);
	return surface;
}