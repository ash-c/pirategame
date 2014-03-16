
// Library Includes

// Local Includes
#include "uibutton.h"
#include "../../input/input.h"

#include "../ui.h"

using namespace Papyrus;

Float32 UI::CUIButton::m_timer = 0.0f;

UI::CUIButton::CUIButton()
	: m_next(0)
	, m_prev(0)
	, m_currState(BUTTON_STATE_NORMAL)
	, m_buttonDown(false)
	, m_stateChanged(false)
{
}

UI::CUIButton::~CUIButton()
{
}

Bool UI::CUIButton::Initialise(Int8* _luaFile, Int8* _luaFunc, Int8* _sprite, VECTOR2 _pos, UInt16 _w, UInt16 _h)
{
	m_sprite = Sprite::CreateSprite(_sprite, 0, false);
	assert(m_sprite);
	m_sprite->AddRef();

	SDL_strlcpy(m_luaFile, _luaFile, MAX_BUFFER);
	SDL_strlcpy(m_luaFunc, _luaFunc, MAX_BUFFER);
	
	m_sprite->SetScale(static_cast<Int32>(_w * UI::wScale), static_cast<Int32>(_h * UI::hScale));
	_pos.x *= UI::wScale;
	_pos.y *= UI::hScale;
	m_sprite->SetPosition(static_cast<Int32>(_pos.x), static_cast<Int32>(_pos.y));
	m_rect.w = _w;
	m_rect.h = _h;
	m_rect.x = static_cast<Int32>(_pos.x - _w * 0.5f);
	m_rect.y = static_cast<Int32>(_pos.y - _h * 0.5f);
	
	for (UInt16 i = 0; i < BUTTON_STATE_MAX; ++i)
	{
		m_clips[i].w = _w;
		m_clips[i].h = _h;
		m_clips[i].x = 0;
		m_clips[i].y = 0;
	}
	
	m_clips[BUTTON_STATE_HOVER].y = _h;
	m_clips[BUTTON_STATE_CLICK].y = _h * 2;

	Input::inputManager->Register(this);
	return true;
}

Bool UI::CUIButton::ShutDown()
{
	Input::inputManager->DeRegister(this);
	VALIDATE(CUIObject::ShutDown());
	return true;
}

void UI::CUIButton::Process(Float32 _delta)
{
	m_stateChanged = false;

	if (m_timer > 0.0f) m_timer -= _delta;
}

void UI::CUIButton::Render()
{
	m_sprite->SetClip(&m_clips[m_currState]);
	m_sprite->Render();
}

void UI::CUIButton::Notify(SDL_Event* _e)
{
	if (m_active)
	{
		if (_e->type == SDL_MOUSEMOTION)
		{
			VECTOR2 mousePos(static_cast<Float32>(_e->motion.x), static_cast<Float32>(_e->motion.y));
		
			if (CheckForHover(mousePos))
			{
				if (m_buttonDown)
				{
					m_currState = BUTTON_STATE_CLICK;
				}
				else
				{
					if (0 != m_next) m_next->SetButtonState(BUTTON_STATE_NORMAL);
					if (0 != m_prev) m_prev->SetButtonState(BUTTON_STATE_NORMAL);
					m_currState = BUTTON_STATE_HOVER;
				}
			}
#ifdef PAPYRUS_EDITOR
			else
			{
				m_currState = BUTTON_STATE_NORMAL;
			}
#else
			else if (Input::inputManager->GetNumControllers() <= 0)
			{
				m_currState = BUTTON_STATE_NORMAL;
			}
#endif // PAPYRUS_EDITOR
		}
		else if (_e->type == SDL_MOUSEBUTTONDOWN)
		{
			m_buttonDown = true;
			if (BUTTON_STATE_HOVER == m_currState)
			{
				m_currState = BUTTON_STATE_CLICK;
				ButtonClicked();
			}
		}
		else if (_e->type == SDL_MOUSEBUTTONUP)
		{
			m_buttonDown = false;
			if (BUTTON_STATE_CLICK == m_currState)
			{
				if (CheckForHover(VECTOR2(static_cast<Float32>(_e->button.x), static_cast<Float32>(_e->button.y))))
				{
					m_currState = BUTTON_STATE_HOVER;
				}
				else
				{
					m_currState = BUTTON_STATE_NORMAL;
				}
			}
		}
		else if (_e->type == SDL_CONTROLLERAXISMOTION)
		{
			if (_e->caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
			{
				if (_e->caxis.value > Input::CONTROLLER_DEAD_ZONE && BUTTON_STATE_HOVER == m_currState && !m_stateChanged && m_timer <= 0.0f)
				{
					if (0 != m_next) m_next->SetButtonState(BUTTON_STATE_HOVER);
					m_currState = BUTTON_STATE_NORMAL;
					m_stateChanged = true;
					m_timer = 0.75f;
				}
				else if (_e->caxis.value < -Input::CONTROLLER_DEAD_ZONE && BUTTON_STATE_HOVER == m_currState && !m_stateChanged && m_timer <= 0.0f)
				{
					if (0 != m_prev) m_prev->SetButtonState(BUTTON_STATE_HOVER);
					m_currState = BUTTON_STATE_NORMAL;
					m_stateChanged = true;
					m_timer = 0.75f;
				}
			}
		}
		else if (_e->cbutton.type == SDL_CONTROLLERBUTTONDOWN)
		{
			if (_e->cbutton.button == SDL_CONTROLLER_BUTTON_A) // Jump or climb a ladder
			{ 
				m_buttonDown = true;
				if (BUTTON_STATE_HOVER == m_currState)
				{
					m_currState = BUTTON_STATE_CLICK;
					ButtonClicked();
				}
			}
		}
	}
}

void UI::CUIButton::SetActive(Bool _b)
{
	m_currState = BUTTON_STATE_NORMAL;
	m_buttonDown = false;

	m_sprite->SetPosition(static_cast<Int32>(m_rect.x + m_rect.w * 0.5f), static_cast<Int32>(m_rect.y + m_rect.h * 0.5f));

	if (_b && !m_active)
	{
		Input::inputManager->Register(this);
	} 
	else if (!_b && m_active)
	{
		Input::inputManager->DeRegister(this);
	}

	CUIObject::SetActive(_b);
}

void UI::CUIButton::SetPosition(VECTOR2 _pos)
{
	m_rect.x = static_cast<Int32>(_pos.x - m_rect.w * 0.5f);
	m_rect.y = static_cast<Int32>(_pos.y - m_rect.h * 0.25f);
	m_sprite->SetPosition(static_cast<Int32>(_pos.x), static_cast<Int32>(_pos.y + m_rect.h * 0.5f));
}

Int32 UI::CUIButton::GetHeight()
{
	return m_rect.h;
}

Int32 UI::CUIButton::GetWidth()
{
	return m_rect.w;
}

void UI::CUIButton::SetButtonState(EButtonState _state) 
{ 
	m_currState = _state; 
	m_stateChanged = true;
}

void UI::CUIButton::SetNext(UI::CUIButton* _next)
{ 
	m_next = _next; 
}

void UI::CUIButton::SetPrev(UI::CUIButton* _prev)
{ 
	m_prev = _prev;
}

Bool UI::CUIButton::CheckForHover(VECTOR2 _mouse)
{
	if (_mouse.x > m_rect.x && _mouse.x < (m_rect.x + m_rect.w) &&
			_mouse.y > m_rect.y && _mouse.y < (m_rect.y + m_rect.h))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UI::CUIButton::ButtonClicked()
{
	// Load file
	luaL_dofile(Logger::luaState, m_luaFile);

	// Get function
	lua_getglobal(Logger::luaState, m_luaFunc);

	lua_call(Logger::luaState, 0, 1);
	const Int8* text = lua_tolstring(Logger::luaState, -1, NULL);
	lua_pop(Logger::luaState, 1);
	Logger::WriteToConsole("%s", text);
	//lua_pop(Logger::luaState, 1);
}