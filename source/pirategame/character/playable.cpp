
// Library Includes

// Local Includes
#include "playable.h"
#include "../parser/parser.h"

using namespace Papyrus;

CPlayable::CPlayable()
	: m_currAnim(ANIM_IDLE_RIGHT)
	, m_actor(0)
{
	m_pos.x = 800;
	m_pos.y = 450;
}

CPlayable::~CPlayable()
{
}

Bool CPlayable::Initialise(Int8* _spriteSheet, Int8* _spriteInfo, Int8* _settings)
{
	assert(0 != _spriteSheet && 0 != _spriteInfo);

	m_sprite = Sprite::CreateSprite(_spriteSheet, _spriteInfo, true);
	assert(m_sprite);
	m_sprite->AddRef();
	m_sprite->SetAnim(m_currAnim);

	Input::inputManager->Register(this);

	FileParser::IParser* settings = FileParser::LoadFile(_settings);
	assert(settings);
	settings->AddRef();

	VECTOR2 max;
	VECTOR2 maxA;
	Float32 mass;
	
	settings->GetValue("maxVel", max);
	settings->GetValue("maxAcc", maxA);
	settings->GetValue("mass", mass);
	settings->GetValue("moveForce", m_moveForce);
	settings->GetValue("slowDownForce", m_slowDownForce);

	m_actor = Physics::CreateDynamicActor(max, maxA, m_pos, m_sprite->GetScale(), mass);
	assert(m_actor);
	m_actor->AddRef();
	settings->Release();

	return true;
}

Bool CPlayable::ShutDown()
{
	m_sprite->Release();
	m_actor->Release();
	return true;
}

void CPlayable::Process(Float32 _delta)
{
	switch (m_currAnim)
	{
	case ANIM_SLIDE_LEFT:
		if (!m_actor->IsActive())
		{
			m_currAnim = ANIM_IDLE_LEFT;
			m_sprite->SetAnim(m_currAnim);
		}
		else 
		{
			m_actor->ApplyForce(VECTOR2(m_slowDownForce.x, 0.0f));
		}
		break;
	case ANIM_SLIDE_RIGHT:
		if (!m_actor->IsActive())
		{
			m_currAnim = ANIM_IDLE_RIGHT;
			m_sprite->SetAnim(m_currAnim);
		}
		else 
		{
			m_actor->ApplyForce(VECTOR2(-m_slowDownForce.x, 0.0f));
		}
		break;
	case ANIM_RUN_LEFT:
		{
			VECTOR2 vel = m_actor->GetVelocity();
			if (vel.x > -m_moveForce.x * 0.5f)
			{
				m_actor->SetVelocity(VECTOR2(-m_moveForce.x * 0.5f, 0.0f));
			}
			m_actor->ApplyForce(VECTOR2(-m_moveForce.x, 0.0f));
		}
		break;
	case ANIM_RUN_RIGHT:
		{
			VECTOR2 vel = m_actor->GetVelocity();
			if (vel.x < m_moveForce.x * 0.5f)
			{
				m_actor->SetVelocity(VECTOR2(m_moveForce.x * 0.5f, 0.0f));
			}
			m_actor->ApplyForce(VECTOR2(m_moveForce.x, 0.0f));
		}
		break;
	default: 
		//m_actor->SetVelocity(VECTOR2(0.0f, 0.0f));
		break;
	}

	m_pos = m_actor->GetPosition();
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
			if (ANIM_SLIDE_RIGHT != m_currAnim)
			{
				m_currAnim = ANIM_RUN_LEFT;
				m_actor->SetActive(true);
			}
			break;
		case SDLK_RIGHT: // Run right
			if (ANIM_SLIDE_LEFT != m_currAnim)
			{
				m_currAnim = ANIM_RUN_RIGHT;
				m_actor->SetActive(true);
			}
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
			//m_currAnim = ANIM_IDLE_LEFT;
			if (ANIM_SLIDE_RIGHT != m_currAnim)
			{
				m_currAnim = ANIM_SLIDE_LEFT;
			}
			break;
		case SDLK_RIGHT:
			//m_currAnim = ANIM_IDLE_RIGHT;
			if (ANIM_SLIDE_LEFT != m_currAnim)
			{
				m_currAnim = ANIM_SLIDE_RIGHT;
			}
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