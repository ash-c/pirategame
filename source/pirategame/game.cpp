
// Library Includes
#include <SDL.h>

// Local Includes
#include "game.h"

CGame::CGame()
	: m_background(0)
	, m_anim(0)
	, m_active(true)
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

	m_background = Sprite::CreateSprite("data/spritesheets/background.png", 0, false);
	assert(m_background);
	m_background->AddRef();
	PY_WRITETOFILE("Background created");

	m_anim = Sprite::CreateSprite("data/spritesheets/spritesheet.png", "data/spritesheets/spritesheet.xml", true);
	assert(m_anim);
	m_anim->AddRef();
	m_anim->SetPosition(800, 300);
	PY_WRITETOFILE("Animiation created");

	// Register the quit function called via the debug console
	lua_register(Logger::luaState, "QuitGame", QuitGame);

	// Finish setup logging, open error logging file.
	PY_WRITETOFILE("Initialistion complete");
	Logger::InitFile("data/papyrus/errors.log");

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
				Logger::ToggleConsole(nullptr);
				break;
			case SDLK_ESCAPE:
				m_active = false;
				break;
			case SDLK_1:
				m_anim->PlayAnim(1);
				Logger::SendInputToConsole(e);
				break;
			default:
				Logger::SendInputToConsole(e);
				break;
			}
		}
	}

	m_anim->Process(_delta);
}

void CGame::Render()
{
	Renderer::Clear();
	
	m_background->Render();

	m_anim->Render();

	// Render core last, as it renders logging.
	Core::Render();

	Renderer::Present();
}

Bool CGame::IsActive()
{
	return m_active;
}

Int32 CGame::QuitGame(lua_State* L)
{
	// Access member variable through the singleton pointer.
	sm_pTheInstance->m_active = false;
	return 0;
}