
// Library Includes
#include <SDL.h>

// Local Includes
#include "game.h"
#include "character\character.h"
#include "character\playable.h"

CGame::CGame()
	: m_levelMan(0)
	, m_interface(0)
	, m_currLevel(0)
	, m_startLevel(0)
	, m_active(true)
	, m_paused(false)
	, m_gameRunning(false)
{
}

CGame::~CGame()
{
	CLEANARRAY(m_currLevel);
	m_levelMan->ShutDown();
	m_levelMan->DestroyInstance();
	m_levelMan = 0;
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
	lua_register(Logger::luaState, "QuitGame", QuitGame);
	lua_register(Logger::luaState, "TitleMenu", TitleMenu);
	lua_register(Logger::luaState, "StartGame", StartGame);
	lua_register(Logger::luaState, "PauseGame", PauseGame);
	lua_register(Logger::luaState, "LoadLevel", LoadLevel);
	lua_register(Logger::luaState, "ToggleCursor", ToggleCursor);

	VALIDATE(Input::inputManager->Register(this));

	FileParser::IParser* levels = FileParser::LoadFile("data/xml/saveState.xml");
	assert(levels);
	levels->AddRef();

	Int32 maxLevels = 0;
	Int32 currLevel = 0;
	VALIDATE(levels->GetValue("totalLevels", maxLevels));
	VALIDATE(levels->GetValue("currLevel", currLevel));
	VALIDATE(levels->GetValue("startLevel", &m_startLevel));

	m_currLevel = new Int8[MAX_BUFFER];
	SDL_snprintf(m_currLevel, MAX_BUFFER, "data/levels/%i.json", currLevel);

	m_levelMan = &CLevelManager::GetInstance();
	VALIDATE(m_levelMan->Initialise(maxLevels, currLevel));
	VALIDATE(m_levelMan->LoadLevel(m_startLevel));

	levels->Release();
	
	m_interface = UI::LoadInterface("data/interfaces/main.ini");
	assert(m_interface);

	// Finish setup logging, open error logging file.
	PY_WRITETOFILE("Initialistion complete");
	Logger::InitFile("data/papyrus/errors.log");

	return true;
}

void CGame::Process(Float32 _delta)
{
	if (0 != m_interface) m_interface->Process(_delta);

	if (!m_paused)
	{
		m_levelMan->Process(_delta);

		if (m_levelMan->IsFinished())
		{
			m_levelMan->Initialise(0, 1);
			m_levelMan->LoadLevel(m_startLevel);

			m_interface->Toggle();
			m_interface = UI::LoadInterface("data/interfaces/main.ini");
			SDL_ShowCursor(true);
		}
	}
}

void CGame::Render()
{
	Renderer::Clear();

	m_levelMan->Render();

	if (0 != m_interface) m_interface->Render();
	
	// Render core last, as it renders logging and physics debug, in that order.
	Core::Render();

	Renderer::Present();
}

Bool CGame::IsActive()
{
	return m_active;
}

void CGame::Pause()
{
	if (m_paused)
	{
		m_interface->Toggle();
		m_interface = UI::LoadInterface("data/interfaces/game.ini");
		SDL_ShowCursor(false);
		Core::Pause();
		m_paused = false;
	}
	else
	{
		m_interface->Toggle();
		m_interface = UI::LoadInterface("data/interfaces/pause.ini");
		SDL_ShowCursor(true);
		Core::Pause();
		m_paused = true;
	}
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
			if (m_gameRunning)
			{
				Pause();
			}
			break;
		case SDLK_BACKQUOTE: // `
			Logger::ToggleConsole(nullptr);
			break;
		default:
			break;
		}
	}
	else if (_e->type == SDL_CONTROLLERBUTTONDOWN)
	{
		if (_e->cbutton.button == SDL_CONTROLLER_BUTTON_START)
		{
			if (m_gameRunning)
			{
				Pause();
			}
		}
	}
}

Int32 CGame::QuitGame(lua_State* L)
{
	// Function is static so can't access m_active normally.
	// Access member variable through the singleton pointer.
	sm_pTheInstance->m_gameRunning = false;
	sm_pTheInstance->m_active = false;
	return 0;
}

Int32 CGame::TitleMenu(lua_State* L)
{
	sm_pTheInstance->m_levelMan->Initialise(0, 1);
	sm_pTheInstance->m_levelMan->LoadLevel(sm_pTheInstance->m_startLevel);

	sm_pTheInstance->m_interface->Toggle();
	sm_pTheInstance->m_interface = UI::LoadInterface("data/interfaces/main.ini");
	SDL_ShowCursor(true);

	sm_pTheInstance->m_gameRunning = false;

	// Because Escape had been hit, game has been paused, so unpause
	Core::Pause();
	sm_pTheInstance->m_paused = false;

	return 0;
}

Int32 CGame::StartGame(lua_State* L)
{
	sm_pTheInstance->m_interface->Toggle();
	sm_pTheInstance->m_interface = UI::LoadInterface("data/interfaces/game.ini");
	SDL_ShowCursor(false);
	sm_pTheInstance->m_levelMan->LoadLevel(sm_pTheInstance->m_currLevel);
	sm_pTheInstance->m_gameRunning = true;
	return 0;
}

Int32 CGame::PauseGame(lua_State* L)
{
	sm_pTheInstance->Pause();
	return 0;
}

Int32 CGame::LoadLevel(lua_State* L)
{
	const Int8* path = luaL_checkstring(L, 1);

	if (sm_pTheInstance->m_levelMan->LoadLevel((Int8*)path))
	{
		sm_pTheInstance->m_interface->Toggle();
		sm_pTheInstance->m_interface = UI::LoadInterface("data/interfaces/game.ini");
		SDL_ShowCursor(0);
	}
	else 
	{
		Logger::WriteToConsole("Failed to load level %s", path);
	}

	return 0;
}

Int32 CGame::ToggleCursor(lua_State* L)
{
	Int32 state = SDL_ShowCursor(-1);

	SDL_ShowCursor(!state);

	return 0;
}