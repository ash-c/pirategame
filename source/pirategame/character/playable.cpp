
// Library Includes

// Local Includes
#include "playable.h"
#include "../parser/parser.h"

using namespace Papyrus;

CPlayable::CPlayable()
	: m_actor(0)
	, m_currAnim(ANIM_IDLE_RIGHT)
	, m_moveDir(MOVE_IDLE)
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

	m_actor = Physics::CreateDynamicActor(max, maxA, m_pos, m_sprite->GetScale(), mass, Physics::EType::TYPE_PLAYER);
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
			m_actor->ApplyForce(VECTOR2(-m_moveForce.x, 0.0f));
		break;
	case ANIM_RUN_RIGHT:
			m_actor->ApplyForce(VECTOR2(m_moveForce.x, 0.0f));
		break;
	case ANIM_IDLE_LEFT:
	case ANIM_IDLE_RIGHT:
		m_moveDir = MOVE_IDLE;
		m_actor->SetActive(false);
		break;
	case ANIM_JUMP_LEFT:
		{
			if (m_actor->IsCollided() && m_actor->GetVelocity().y >= 0.0f)
			{
				if (MOVE_RIGHT == m_moveDir)
				{
					m_currAnim = ANIM_RUN_RIGHT;
				}
				else if (MOVE_LEFT == m_moveDir)
				{
					m_currAnim = ANIM_RUN_LEFT;
				}
				else if (MOVE_IDLE == m_moveDir)
				{
					m_currAnim = ANIM_SLIDE_LEFT;
				}
				m_moveDir = MOVE_IDLE;
				m_sprite->SetAnim(m_currAnim);
			}
			if (m_actor->GetVelocity().x > 0.0f) // Switch animation for direction change
			{
				m_currAnim = ANIM_JUMP_RIGHT;
				m_moveDir = MOVE_RIGHT;
			}
			if (MOVE_LEFT == m_moveDir)
			{
				m_actor->ApplyForce(VECTOR2(-m_moveForce.x, 0.0f));
			} 
			else if (MOVE_RIGHT == m_moveDir)
			{
				m_actor->ApplyForce(VECTOR2(m_moveForce.x, 0.0f));
			}
		}
		break;
	case ANIM_JUMP_RIGHT:
		{
			if (m_actor->IsCollided() && m_actor->GetVelocity().y >= 0.0f)
			{
				if (MOVE_RIGHT == m_moveDir)
				{
					m_currAnim = ANIM_RUN_RIGHT;
				}
				else if (MOVE_LEFT == m_moveDir)
				{
					m_currAnim = ANIM_RUN_LEFT;
				}
				else if (MOVE_IDLE == m_moveDir)
				{
					m_currAnim = ANIM_SLIDE_RIGHT;
				}
				m_moveDir = MOVE_IDLE;
				m_sprite->SetAnim(m_currAnim);
			}
			if (m_actor->GetVelocity().x < 0.0f) // Switch animation for direction change
			{
				m_currAnim = ANIM_JUMP_LEFT;
				m_moveDir = MOVE_LEFT;
			}
			if (MOVE_LEFT == m_moveDir)
			{
				m_actor->ApplyForce(VECTOR2(-m_moveForce.x, 0.0f));
			} 
			else if (MOVE_RIGHT == m_moveDir)
			{
				m_actor->ApplyForce(VECTOR2(m_moveForce.x, 0.0f));
			}
		}
		break;
	default: 
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

void CPlayable::SetPosition(VECTOR2 _v)
{
	m_actor->SetPosition(_v);
}

void CPlayable::Notify(SDL_Event* _e)
{
	VECTOR2 vel = m_actor->GetVelocity();
	if (_e->type == SDL_KEYDOWN)
	{
		switch (_e->key.keysym.sym)
		{
		case SDLK_LEFT: // Run left
			{
				if (ANIM_SLIDE_RIGHT != m_currAnim && ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
				{
					m_currAnim = ANIM_RUN_LEFT;
					if (vel.x > -m_moveForce.x)
					{
						m_actor->SetVelocity(VECTOR2(-m_moveForce.x, vel.y));
					}
				}
				m_actor->SetActive(true);
				m_moveDir = MOVE_LEFT;
			}
			break;
		case SDLK_RIGHT: // Run right
			{
				if (ANIM_SLIDE_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim && ANIM_JUMP_LEFT != m_currAnim)
				{
					m_currAnim = ANIM_RUN_RIGHT;
					if (vel.x < m_moveForce.x)
					{
						m_actor->SetVelocity(VECTOR2(m_moveForce.x, vel.y));
					}
				}
				m_actor->SetActive(true);
				m_moveDir = MOVE_RIGHT;
			}
			break; 
		case SDLK_UP: // Jump or climb a ladder
			if (ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
			{
				if (ANIM_IDLE_LEFT == m_currAnim || ANIM_RUN_LEFT == m_currAnim || ANIM_SLIDE_LEFT == m_currAnim || ANIM_ATTACK_LEFT == m_currAnim)
				{
					m_currAnim = ANIM_JUMP_LEFT;
					m_actor->ApplyForce(VECTOR2(0.0f, -47000.0f));
					m_actor->SetVelocity(VECTOR2(vel.x, -m_moveForce.y * 2.0f));
				} 
				else 
				{
					m_currAnim = ANIM_JUMP_RIGHT;
					m_actor->ApplyForce(VECTOR2(0.0f, -47000.0f));
					m_actor->SetVelocity(VECTOR2(vel.x, -m_moveForce.y * 2.0f));
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
			if (ANIM_SLIDE_RIGHT != m_currAnim)
			{
				if (ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
				{
					m_currAnim = ANIM_SLIDE_LEFT;
				}
				m_moveDir = MOVE_IDLE;
			}
			break;
		case SDLK_RIGHT:
			if (ANIM_SLIDE_LEFT != m_currAnim)
			{
				if (ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
				{
					m_currAnim = ANIM_SLIDE_RIGHT;
				}
				m_moveDir = MOVE_IDLE;
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
			if(_e->caxis.value < -Input::CONTROLLER_DEAD_ZONE && ANIM_SLIDE_RIGHT != m_currAnim && ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim) // left.
			{
				m_currAnim = ANIM_RUN_LEFT;
				m_moveDir = MOVE_LEFT;
				m_actor->SetActive(true);
			}
			else if (_e->caxis.value > Input::CONTROLLER_DEAD_ZONE && ANIM_SLIDE_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim && ANIM_JUMP_LEFT != m_currAnim) // right.
			{
				m_currAnim = ANIM_RUN_RIGHT;
				m_moveDir = MOVE_RIGHT;
				m_actor->SetActive(true);
			}
			else if (ANIM_SLIDE_RIGHT != m_currAnim && ANIM_SLIDE_LEFT != m_currAnim) // Idle
			{
				if (ANIM_RUN_LEFT == m_currAnim)
				{
					m_currAnim = ANIM_SLIDE_LEFT;
					m_moveDir = MOVE_IDLE;
				}
				else if (ANIM_RUN_RIGHT == m_currAnim)
				{
					m_currAnim = ANIM_SLIDE_RIGHT;
					m_moveDir = MOVE_IDLE;
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
				if (ANIM_IDLE_LEFT == m_currAnim || ANIM_RUN_LEFT == m_currAnim || ANIM_SLIDE_LEFT == m_currAnim || ANIM_ATTACK_LEFT == m_currAnim)
				{
					m_currAnim = ANIM_JUMP_LEFT;
					m_actor->ApplyForce(VECTOR2(0.0f, -50000.0f));
					m_actor->SetVelocity(VECTOR2(vel.x, -m_moveForce.y));
				} 
				else 
				{
					m_currAnim = ANIM_JUMP_RIGHT;
					m_actor->ApplyForce(VECTOR2(0.0f, -50000.0f));
					m_actor->SetVelocity(VECTOR2(vel.x, -m_moveForce.y));
				}
			}
		}
	}
	m_sprite->SetAnim(m_currAnim);
}