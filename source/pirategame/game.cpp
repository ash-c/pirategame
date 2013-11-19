
// Library Includes
#include <SDL.h>

// Local Includes
#include "game.h"

CGame::CGame()
	: m_background(0)
	, m_anim(0)
	, m_active(true)
{
	m_pos.x = 1600/2;
	m_pos.y = 900/2;
}

CGame::~CGame()
{
	SDL_JoystickClose(m_joystick);
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
	m_anim->SetPosition(static_cast<Int32>(m_pos.x), static_cast<Int32>(m_pos.y));
	PY_WRITETOFILE("Animiation created");

	// Register the quit function called via the debug console
	lua_register(Logger::luaState, "QuitGame", QuitGame);

	// Finish setup logging, open error logging file.
	PY_WRITETOFILE("Initialistion complete");
	Logger::InitFile("data/papyrus/errors.log");

	Input::inputManager->Register(this);

	m_joystick = SDL_JoystickOpen(0);
	assert(m_joystick);

	return true;
}

void CGame::Process(Float32 _delta)
{
	m_anim->SetPosition(static_cast<Int32>(m_pos.x), static_cast<Int32>(m_pos.y));
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
		m_active = false;			
	else if (_e->type == SDL_KEYDOWN)
	{
		switch (_e->key.keysym.sym)
		{
		case SDLK_BACKQUOTE: // `
			Logger::ToggleConsole(nullptr);
			break;
		case SDLK_ESCAPE:
			m_active = false;
			break;
		case SDLK_LEFT:
			m_pos.x -= speed * 1/60;
			m_anim->SetAnim(1);
			break;
		case SDLK_RIGHT:
			m_pos.x += speed * 1/60;
			m_anim->SetAnim(1);
			break;
		case SDLK_UP:
			m_pos.y -= speed * 1/60;
			m_anim->SetAnim(1);
			break;
		case SDLK_DOWN:
			m_pos.y += speed * 1/60;
			m_anim->SetAnim(1);
			break;
		}
	}
	else if (_e->type == SDL_KEYUP)
	{
		switch (_e->key.keysym.sym)
		{
		case SDLK_LEFT:
		case SDLK_RIGHT:
		case SDLK_UP:
		case SDLK_DOWN:
			m_anim->SetAnim(0);
			break;
		}
	}
	else if (_e->type == SDL_JOYAXISMOTION)
	{
		if (_e->caxis.axis == 0)
		{
			if(_e->caxis.value < -10000) // left.
			{
				m_pos.x -= speed * 1/60;
				m_anim->SetAnim(1);
			}
			else if (_e->caxis.value > 10000) // right.
			{
				m_pos.x += speed * 1/60;
				m_anim->SetAnim(1);
			}
			else
			{
				m_anim->SetAnim(0);
			}
		}
		if (_e->caxis.axis == 1)
		{
			if(_e->caxis.value < -10000) // up.
			{
				m_pos.y -= speed * 1/60;
				m_anim->SetAnim(1);
			}
			else if (_e->caxis.value > 10000) // down.
			{
				m_pos.y += speed * 1/60;
				m_anim->SetAnim(1);
			}
			else
			{
				m_anim->SetAnim(0);
			}
		}
	}

}

Int32 CGame::QuitGame(lua_State* L)
{
	// Access member variable through the singleton pointer.
	sm_pTheInstance->m_active = false;
	return 0;
}