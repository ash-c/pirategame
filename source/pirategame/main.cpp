
#include "game.h"

using namespace std;

// Square tiles
const int TILE_SIZE = 40;

int main(int argc, char **argv)
{
	CGame& game = CGame::GetInstance();

	if (!game.Initialise())
	{
		// Failed to initialise. Need to do some logging.
		return 1;
	}

	while (game.IsActive())
	{
		game.ExecuteOneFrame();
	}

	game.DestroyInstance();

	return 0;
}