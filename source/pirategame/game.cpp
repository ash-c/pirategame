
// Library Includes
#include <SDL.h>

// Local Includes
#include "game.h"

using namespace Papyrus;

CGame::CGame()
	: background(0)
	, active(true)
{
	
}

CGame::~CGame()
{
	background->Release();
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

	background = Sprite::CreateSprite("data/spritesheets/background.png", false);
	background->AddRef();

	return true;
}

void CGame::Process()
{
	Core::Process();

	SDL_Event e;
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

void CGame::Render()
{
	Renderer::Clear();

	background->Render();

	Renderer::Present();
}

Bool CGame::IsActive()
{
	return active;
}