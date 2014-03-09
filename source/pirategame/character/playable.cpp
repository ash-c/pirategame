
// Library Includes

// Local Includes
#include "playable.h"
#include "../../parser/parser.h"

#include "../level/level.h"
#include "../level/levelmanager.h"

using namespace Papyrus;

CPlayable::CPlayable()
	: m_moveDir(MOVE_IDLE)
{
	m_pos.x = 0;
	m_pos.y = 0;
}

CPlayable::~CPlayable()
{
}

Bool CPlayable::Initialise(Int8* _spriteSheet, Int8* _spriteInfo, Int8* _settings)
{
	assert(0 != _spriteSheet && 0 != _spriteInfo);
	m_currAnim = ANIM_IDLE_RIGHT;
	m_moveDir = MOVE_IDLE;

	if (0 == m_sprite)
	{
		m_sprite = Sprite::CreateSprite(_spriteSheet, _spriteInfo, true);
		assert(m_sprite);
		m_sprite->AddRef();

		Input::inputManager->Register(this);
	}
	m_sprite->SetAnim(m_currAnim);

	FileParser::IParser* settings = FileParser::LoadFile(_settings);
	assert(settings);
	settings->AddRef();

	VECTOR2 max;
	VECTOR2 maxA;
	Float32 mass;
	
	VALIDATE(settings->GetValue("maxVel", max));
	VALIDATE(settings->GetValue("maxAcc", maxA));
	VALIDATE(settings->GetValue("mass", mass));
	VALIDATE(settings->GetValue("moveForce", m_moveForce));
	VALIDATE(settings->GetValue("slowDownForce", m_slowDownForce));

	VECTOR2 scale = m_sprite->GetScale();
	scale.x *= 0.65f;

	if (0 == m_actor)
	{
		m_actor = Physics::CreateDynamicActor(max, maxA, m_pos, scale, mass, Physics::EType::TYPE_PLAYER);
		assert(m_actor);
		m_actor->AddRef();
		m_actor->SetOwner((void*)this);
	}
	m_actor->SetVelocity(VECTOR2(0.0f, 0.0f));
	m_actor->SetActive(true);
	m_actor->SetVCollided(true);
	settings->Release();

	return true;
}

Bool CPlayable::ShutDown()
{
	Input::inputManager->DeRegister(this);
	PY_SAFE_RELEASE(m_sprite);
	Physics::FlushActor(m_actor);
	return true;
}

void CPlayable::Process(Float32 _delta)
{
#ifndef PAPYRUS_EDITOR
	EAnims currPlayed = static_cast<EAnims>(m_sprite->GetAnim());

	if (m_actor->IsPECollided() && ANIM_DEATH_LEFT != m_currAnim && ANIM_DEATH_RIGHT != m_currAnim)// && ANIM_ATTACK_LEFT != currPlayed && ANIM_ATTACK_RIGHT != currPlayed)
	{
		if (ANIM_IDLE_LEFT == m_currAnim || ANIM_RUN_LEFT == m_currAnim || ANIM_JUMP_LEFT == m_currAnim || ANIM_FALL_LEFT == m_currAnim || ANIM_ATTACK_LEFT == m_currAnim)
		{
			m_sprite->PlayAnim(ANIM_DEATH_LEFT, true);
			m_currAnim = ANIM_DEATH_LEFT;
		}
		else if (ANIM_IDLE_RIGHT == m_currAnim || ANIM_RUN_RIGHT == m_currAnim || ANIM_JUMP_RIGHT == m_currAnim || ANIM_FALL_RIGHT == m_currAnim || ANIM_ATTACK_RIGHT == m_currAnim)
		{
			m_sprite->PlayAnim(ANIM_DEATH_RIGHT, true);
			m_currAnim = ANIM_DEATH_RIGHT;
		}
		return;
	}
	if (ANIM_DEATH_LEFT == m_currAnim || ANIM_DEATH_RIGHT == m_currAnim)
	{		
		if (ANIM_DEATH_LEFT != currPlayed && ANIM_DEATH_RIGHT != currPlayed)
		{
			m_actor->SetActive(false);
			CLevelManager::RestartLevel(0);
		}
		else
		{
			m_sprite->Process(_delta);
		}
		return;
	}
	
	VECTOR2 vel = m_actor->GetVelocity();

	switch (m_currAnim)
	{
	case ANIM_SLIDE_LEFT:
		if (m_actor->IsStationary())
		{
			m_currAnim = ANIM_IDLE_LEFT;
			m_sprite->SetAnim(m_currAnim);
		}
		else 
		{
			m_actor->SetVelocity(VECTOR2(vel.x * m_slowDownForce.x, vel.y));
			//m_actor->ApplyForce(VECTOR2(m_slowDownForce.x, 0.0f));
		}
		break;
	case ANIM_SLIDE_RIGHT:
		if (m_actor->IsStationary())
		{
			m_currAnim = ANIM_IDLE_RIGHT;
			m_sprite->SetAnim(m_currAnim);
		}
		else 
		{
			m_actor->SetVelocity(VECTOR2(vel.x * m_slowDownForce.x, vel.y));
			//m_actor->ApplyForce(VECTOR2(-m_slowDownForce.x, 0.0f));
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
		{
			m_moveDir = MOVE_IDLE;
		}
		break;
	case ANIM_ATTACK_LEFT:
	case ANIM_ATTACK_RIGHT:
		{
			if (ANIM_ATTACK_LEFT != currPlayed && ANIM_ATTACK_RIGHT != currPlayed)
			{
				m_currAnim = currPlayed;
				//VECTOR2 scale = m_sprite->GetScale();
				//m_actor->SetScale(scale);
			}
		}
		break;
	case ANIM_JUMP_LEFT:
		{
			if (m_actor->IsVCollided() && m_actor->GetVelocity().y >= 0.0f)
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
				m_sprite->SetAnim(m_currAnim);
			}
			else if (m_actor->GetVelocity().x > 0.0f) // Switch animation for direction change
			{
				m_currAnim = ANIM_JUMP_RIGHT;
				m_sprite->SetAnim(m_currAnim);
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
			if (m_actor->IsVCollided() && m_actor->GetVelocity().y >= 0.0f)
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
				m_sprite->SetAnim(m_currAnim);
			}
			else if (m_actor->GetVelocity().x < 0.0f) // Switch animation for direction change
			{
				m_currAnim = ANIM_JUMP_LEFT;
				m_sprite->SetAnim(m_currAnim);
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
	m_sprite->Process(_delta);
#endif // PAPYRUS_EDITOR
}

void CPlayable::Render(VECTOR2 _camPos)
{
	m_sprite->SetPosition(static_cast<Int32>(m_pos.x + _camPos.x), static_cast<Int32>(m_pos.y - _camPos.y));
	m_sprite->Render();
}

void CPlayable::SetPosition(VECTOR2 _v)
{
	m_actor->SetPosition(_v);
//#ifdef PAPYRUS_EDITOR
	m_pos = _v;
	m_sprite->SetPosition(static_cast<Int32>(_v.x), static_cast<Int32>(_v.y));
//#endif // PAPYRUS_EDITOR
}

void CPlayable::Notify(SDL_Event* _e)
{
	// don't take input if death animation is playing.
	if (ANIM_DEATH_LEFT == m_currAnim || ANIM_DEATH_RIGHT == m_currAnim) return;

	VECTOR2 vel = m_actor->GetVelocity();
	if (_e->type == SDL_KEYDOWN)
	{
		switch (_e->key.keysym.sym)
		{
		case SDLK_LEFT: // Run left
			Move(&vel, true);
			break;
		case SDLK_RIGHT: // Run right
			Move(&vel, false);
			break; 
		case SDLK_UP: // Jump or climb a ladder
			Jump(&vel);
			break;
		case SDLK_SPACE: // Attack
			Attack();
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
			StopMove(true);
			break;
		case SDLK_RIGHT:
			StopMove(false);
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
				Move(&vel, true);
			}
			else if (_e->caxis.value > Input::CONTROLLER_DEAD_ZONE) // right.
			{
				Move(&vel, false);
			}
			else if (ANIM_SLIDE_RIGHT != m_currAnim && ANIM_SLIDE_LEFT != m_currAnim) // Idle
			{
				if (ANIM_RUN_LEFT == m_currAnim || ANIM_JUMP_LEFT == m_currAnim)
				{
					StopMove(true);
				}
				else if (ANIM_RUN_RIGHT == m_currAnim || ANIM_JUMP_RIGHT == m_currAnim)
				{
					StopMove(false);
				}
			}
		}
	}
	else if (_e->cbutton.type == SDL_CONTROLLERBUTTONDOWN)
	{
		if (_e->cbutton.button == SDL_CONTROLLER_BUTTON_A) // Jump or climb a ladder
		{ 
			Jump(&vel);
		}
		else if (_e->cbutton.button == SDL_CONTROLLER_BUTTON_B) // Attack
		{
			Attack();
		}
	}
}

void CPlayable::Move(VECTOR2* _vel, Bool _left)
{
	if (_left)
	{
		//if (ANIM_SLIDE_RIGHT != m_currAnim && ANIM_SLIDE_LEFT != m_currAnim)
		//{
			if (ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
			{
				m_currAnim = ANIM_RUN_LEFT;
			}
			if (_vel->x > -m_moveForce.x)
			{
				m_actor->SetVelocity(VECTOR2(-m_moveForce.x, _vel->y));
			}
				
			m_moveDir = MOVE_LEFT;
		//}
	} 
	else
	{
		//if (ANIM_SLIDE_RIGHT != m_currAnim && ANIM_SLIDE_LEFT != m_currAnim)
		//{
			if (ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
			{
				m_currAnim = ANIM_RUN_RIGHT;
			}
			if (_vel->x < m_moveForce.x)
			{
				m_actor->SetVelocity(VECTOR2(m_moveForce.x, _vel->y));
			}
				
			m_moveDir = MOVE_RIGHT;
		//}
	}
	m_sprite->SetAnim(m_currAnim);
}

void CPlayable::StopMove(Bool _left)
{
	if (_left)
	{
		if (ANIM_SLIDE_RIGHT != m_currAnim && ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
		{
			if (ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
			{
				m_currAnim = ANIM_SLIDE_LEFT;
			}
			m_moveDir = MOVE_IDLE;
		}
		else if (ANIM_JUMP_LEFT == m_currAnim)
		{
			m_moveDir = MOVE_IDLE;
		}
	}
	else
	{
		if (ANIM_SLIDE_LEFT != m_currAnim && ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
		{
			if (ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
			{
				m_currAnim = ANIM_SLIDE_RIGHT;
			}
			m_moveDir = MOVE_IDLE;
		}
		else if (ANIM_JUMP_RIGHT == m_currAnim)
		{
			m_moveDir = MOVE_IDLE;
		}
	}
	m_sprite->SetAnim(m_currAnim);
}

void CPlayable::Jump(VECTOR2* _vel)
{
	if (ANIM_JUMP_LEFT != m_currAnim && ANIM_JUMP_RIGHT != m_currAnim)
	{
		if (ANIM_IDLE_LEFT == m_currAnim || ANIM_RUN_LEFT == m_currAnim || ANIM_SLIDE_LEFT == m_currAnim || ANIM_ATTACK_LEFT == m_currAnim)
		{
			m_currAnim = ANIM_JUMP_LEFT;
			m_actor->SetVelocity(VECTOR2(_vel->x, -m_moveForce.y));
			m_actor->SetPosition(VECTOR2(m_pos.x, m_pos.y - 10.0f)); // subtracting extra to ensure no collision with platform
		} 
		else if (ANIM_IDLE_RIGHT == m_currAnim || ANIM_RUN_RIGHT == m_currAnim || ANIM_SLIDE_RIGHT == m_currAnim || ANIM_ATTACK_RIGHT == m_currAnim)
		{
			m_currAnim = ANIM_JUMP_RIGHT;
			m_actor->SetVelocity(VECTOR2(_vel->x, -m_moveForce.y));
			m_actor->SetPosition(VECTOR2(m_pos.x, m_pos.y - 10.0f));
		}
	}
	m_sprite->SetAnim(m_currAnim);
}

void CPlayable::Attack()
{
	if (ANIM_ATTACK_RIGHT != m_currAnim && ANIM_ATTACK_LEFT != m_currAnim)
	{
		VECTOR2 scale = m_sprite->GetScale();
		// attack left
		if (ANIM_IDLE_LEFT == m_currAnim || ANIM_RUN_LEFT == m_currAnim || ANIM_JUMP_LEFT == m_currAnim ||
				ANIM_FALL_LEFT == m_currAnim || ANIM_SLIDE_LEFT == m_currAnim)
		{
			m_currAnim = ANIM_ATTACK_LEFT;
			m_sprite->PlayAnim(ANIM_ATTACK_LEFT);
		} 
		// attack right
		else if (ANIM_IDLE_RIGHT == m_currAnim || ANIM_RUN_RIGHT == m_currAnim || ANIM_JUMP_RIGHT == m_currAnim ||
				ANIM_FALL_RIGHT == m_currAnim || ANIM_SLIDE_RIGHT == m_currAnim)
		{
			m_currAnim = ANIM_ATTACK_RIGHT;
			m_sprite->PlayAnim(ANIM_ATTACK_RIGHT);
		}
	}
}