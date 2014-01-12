
// Library Includes
#include <SDL.h>

// Local Includes
#include "game.h"

CGame::CGame()
	: m_interface(0)
	, m_active(true)
	, m_paused(false)
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

	// Register the quit function called via the debug console

	VALIDATE(Input::inputManager->Register(this));

	// Finish setup logging, open error logging file.
	PY_WRITETOFILE("Initialistion complete");
	Logger::InitFile("data/papyrus/errors.log");

	return true;
}

void CGame::Process(Float32 _delta)
{
	if (!m_paused)
	{
	}
}

void CGame::Render()
{
	Renderer::Clear();
	
	if (0 != m_interface)
	{
		m_interface->Render();
	}
	
	// Render core last, as it renders logging and physics debug, in that order.
	Core::Render();

	Renderer::Present();
}

Bool CGame::IsActive()
{
	return m_active;
}

void CGame::Notify(SDL_Event* _e)
{
	if (_e->type == SDL_QUIT)
	{
		m_active = false;			
	}
	else if (_e->type == SDL_KEYDOWN)
	{
		switch (_e->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			m_active = false;
			break;
		case SDLK_BACKQUOTE: // `
			Logger::ToggleConsole(nullptr);
			break;
		default:
			break;
		}
	}
}