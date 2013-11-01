#include <vld.h>

#include "game.h"

using namespace std;

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