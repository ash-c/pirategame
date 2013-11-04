
// Library Includes
#include <SDL.h>

// Local Includes
#include "game.h"

CGame::CGame()
	: m_background(0)
	, m_active(true)
{
	
}

CGame::~CGame()
{
	PY_RELEASE(m_background);

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

	m_background = Sprite::CreateSprite("data/spritesheets/background.png", false);
	assert(m_background);
	m_background->AddRef();

	PY_WRITETOFILE("Initialistion complete");
	Logger::InitFile("data/errors.log");

	return true;
}

void CGame::Process(Float32 _delta)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			m_active = false;			
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_BACKQUOTE: // `
				Logger::ToggleConsole();
				break;
			case SDLK_1:
				PY_WRITETOCONSOLE("Console test message");
				break;
			case SDLK_ESCAPE:
				m_active = false;
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
	
	m_background->Render();

	// Render core last, as it renders logging.
	Core::Render();

	Renderer::Present();
}

Bool CGame::IsActive()
{
	return m_active;
}