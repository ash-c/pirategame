
#include "game.h"

using namespace std;

// Square tiles
const int TILE_SIZE = 40;

int main(int argc, char **argv)
{
	CGame& game = CGame::GetInstance();

	if (!game.Initialise())
	{
		// Failed to initialise. Should do some windows based logging of some sort?
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

	renderTexture(image, renderer, x, y, &clips[useClip]);		
	SDL_DestroyTexture(image);
	*/

	return 0;
}