
// Library Includes

// Local Includes
#include "playable.h"

using namespace Papyrus;

CPlayable::CPlayable()
	: m_currAnim(ANIM_IDLE_RIGHT)
{
	m_pos.x = 800;
	m_pos.y = 450;
}

CPlayable::~CPlayable()
{
}

Bool CPlayable::Initialise(Int8* _spriteSheet, Int8* _spriteInfo)
{
	assert(0 != _spriteSheet && 0 != _spriteInfo);

	m_sprite = Sprite::CreateSprite(_spriteSheet, _spriteInfo, true);
	assert(m_sprite);
	m_sprite->AddRef();
	m_sprite->SetAnim(m_currAnim);

	Input::inputManager->Register(this);

	return true;
}

Bool CPlayable::ShutDown()
{
	return true;
}

void CPlayable::Process(Float32 _delta)
{
	switch (m_currAnim)
	{
	case ANIM_RUN_LEFT:
		m_pos.x -= SPEED * _delta;
		break;
	case ANIM_RUN_RIGHT:
		m_pos.x += SPEED * _delta;
		break;
	default: 
		break;
	}

	m_sprite->SetPosition(static_cast<Int32>(m_pos.x), static_cast<Int32>(m_pos.y));
	m_sprite->Process(_delta);
}

void CPlayable::Render()
{
	m_sprite->Render();
}

void CPlayable::Notify(SDL_Event* _e)
{
	if (_e->type == SDL_KEYDOWN)
	{
		switch (_e->key.keysym.sym)
		{
		case SDLK_LEFT: // Run left
			m_currAnim = ANIM_RUN_LEFT;
			break;
		case SDLK_RIGHT: // Run right
			m_currAnim = ANIM_RUN_RIGHT;
			break; 
		case SDLK_UP: // Jump or climb a ladder
			if (ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
			{
				if (ANIM_IDLE_LEFT == m_currAnim || ANIM_RUN_LEFT == m_currAnim)
				{
					m_currAnim = ANIM_JUMP_LEFT;
				} 
				else 
				{
					m_currAnim = ANIM_JUMP_RIGHT;
				}
			}
			break;
		default:
			break;
		}
	}
	else if (_e->type == SDL_KEYUP)
	{
		switch(_e->key.keysym.sym)
		{
		case SDLK_LEFT:
			m_currAnim = ANIM_IDLE_LEFT;
			break;
		case SDLK_RIGHT:
			m_currAnim = ANIM_IDLE_RIGHT;
			break;
		default:
			break;
		}
	}
	else if (_e->type == SDL_CONTROLLERAXISMOTION)
	{
		if (_e->caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) // x axis on the left stick
		{
			if(_e->caxis.value < -Input::CONTROLLER_DEAD_ZONE) // left.
			{
				m_currAnim = ANIM_RUN_LEFT;
			}
			else if (_e->caxis.value > Input::CONTROLLER_DEAD_ZONE) // right.
			{
				m_currAnim = ANIM_RUN_RIGHT;
			}
			else // Idle
			{
				if (ANIM_RUN_LEFT == m_currAnim || ANIM_IDLE_LEFT == m_currAnim)
				{
					m_currAnim = ANIM_IDLE_LEFT;
				}
				else if (ANIM_RUN_RIGHT == m_currAnim || ANIM_IDLE_RIGHT == m_currAnim)
				{
					m_currAnim = ANIM_IDLE_RIGHT;
				}
			}
		}
	}
	else if (_e->cbutton.type == SDL_CONTROLLERBUTTONDOWN)
	{
		if (_e->cbutton.button == SDL_CONTROLLER_BUTTON_A) // Jump or climb a ladder
		{ 
			if (ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
			{
				if (ANIM_IDLE_LEFT == m_currAnim || ANIM_RUN_LEFT == m_currAnim)
				{
					m_currAnim = ANIM_JUMP_LEFT;
				} 
				else 
				{
					m_currAnim = ANIM_JUMP_RIGHT;
				}
			}
		}
	}
	m_sprite->SetAnim(m_currAnim);
}