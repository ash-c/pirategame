
// Library Includes
#include <SDL.h>

// Local Includes
#include "game.h"
#include "character\character.h"
#include "character\playable.h"

CGame::CGame()
	: m_background(0)
	, m_anim(0)
	, m_active(true)
{
	m_pos.x = 1920/2;
	m_pos.y = 1080/2;
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

	m_background = Sprite::CreateSprite("data/art/background.png", 0, false);
	assert(m_background);
	m_background->AddRef();
	PY_WRITETOFILE("Background created");

	CREATEPOINTER(m_anim, CPlayable);
	assert(m_anim);
	m_anim->Initialise("data/art/characters/sam/male.png", "data/art/characters/sam/male.xml", "data/xml/characters/sam.xml");
	PY_WRITETOFILE("Animiation created");

	// Register the quit function called via the debug console
	lua_register(Logger::luaState, "QuitGame", QuitGame);

	// Finish setup logging, open error logging file.
	PY_WRITETOFILE("Initialistion complete");
	Logger::InitFile("data/papyrus/errors.log");

	Input::inputManager->Register(this);

	return true;
}

void CGame::Process(Float32 _delta)
{
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

void CGame::Notify(SDL_Event* _e)
{
	Int32 speed = 400;

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

Int32 CGame::QuitGame(lua_State* L)
{
	// Function is static so can't access m_active normally.
	// Access member variable through the singleton pointer.
	sm_pTheInstance->m_active = false;
	return 0;
}