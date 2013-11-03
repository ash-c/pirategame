
// Library Includes
#include <SDL.h>

// Local Includes
#include "game.h"

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
	Process(Core::Process());
	Render();
}

Bool CGame::Initialise()
{
	VALIDATE(Core::Initialise());

	return true;
}

void CGame::Process(Float32 _delta)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			active = false;			
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_BACKQUOTE: // `
				Logger::ToggleScreenLogging();
				break;
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

	Core::Render();

	Renderer::Present();
}

Bool CGame::IsActive()
{
	return active;
}