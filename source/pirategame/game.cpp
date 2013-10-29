
// Library Includes
#include <SDL.h>

// Local Includes
#include "game.h"
#include "papyrus\core\core.h"
#include "papyrus\renderer\renderer.h"

using namespace Papyrus;

CGame::CGame()
	: active(true)
{
	
}

CGame::~CGame()
{
	Core::ShutDown();
}

void CGame::ExecuteOneFrame()
{
	Process();
	Render();
}

Bool CGame::Initialise()
{
	Core::Initialise(10);

	return true;
}

void CGame::Process()
{
	Core::Process();

	SDL_Event e;
	while (active)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				active = false;			
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					active = false;
					break;
				default:
					break;
				}
			}
		}
	}
}

void CGame::Render()
{
	Renderer::Clear();

	Renderer::Present();
}

Bool CGame::IsActive()
{
	return active;
}