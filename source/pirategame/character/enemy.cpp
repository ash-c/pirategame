// Library Includes

// Local Includes
#include "enemy.h"

CEnemy::CEnemy()
	: m_player(0)
	, m_attackDelay(0.0f)
	, m_screenW(0)
	, m_screenH(0)
	, m_left(true)
	, m_alive(true)
{
}

CEnemy::~CEnemy()
{
}

Bool CEnemy::Initialise(Int8* _spriteSheet, Int8* _spriteInfo, Int8* _settings)
{
	assert(0 != _spriteSheet && 0 != _spriteInfo);

#ifdef PAPYRUS_EDITOR
	SDL_snprintf(m_spriteSheet, MAX_BUFFER, _spriteSheet);
	SDL_snprintf(m_spriteSettings, MAX_BUFFER, _spriteInfo);
	SDL_snprintf(m_settings, MAX_BUFFER, _settings);
#endif // PAPYRUS_EDITOR

	m_left = true;
	m_alive = true;

	m_currAnim = ANIM_RUN_LEFT;
	if (0 == m_sprite)
	{
		m_sprite = Sprite::CreateSprite(_spriteSheet, _spriteInfo, true);
		assert(m_sprite);
		m_sprite->AddRef();
	}
	m_sprite->SetAnim(m_currAnim);

	FileParser::IParser* settings = FileParser::LoadFile(_settings);
	assert(settings);
	settings->AddRef();

	VECTOR2 max;
	VECTOR2 maxA;
	Float32 mass;
	Int32 type;
	
	VALIDATE(settings->GetValue("maxVel", max));
	VALIDATE(settings->GetValue("maxAcc", maxA));
	VALIDATE(settings->GetValue("mass", mass));
	VALIDATE(settings->GetValue("moveForce", m_moveForce));
	VALIDATE(settings->GetValue("slowDownForce", m_slowDownForce));
	VALIDATE(settings->GetValue("type", type));

	VECTOR2 scale = m_sprite->GetScale();
	if (0 == m_actor)
	{
		m_actor = Physics::CreateDynamicActor(max, maxA, m_pos, scale, mass, static_cast<Physics::EType>(type));
		assert(m_actor);
		m_actor->AddRef();
		m_actor->SetOwner((void*)this);
	}
	else 
	{
		m_actor->SetPosition(m_pos);
	}

	m_actor->SetVCollided(true);
	m_actor->SetVelocity(VECTOR2(-m_moveForce.x, 0.0f));
	settings->Release();
	
	m_screenW = Renderer::activeRenderer->GetWidth();
	m_screenH = Renderer::activeRenderer->GetHeight();

	return true;
}

Bool CEnemy::ShutDown()
{
	PY_SAFE_RELEASE(m_sprite);
	Physics::FlushActor(m_actor);
	return true;
}

void CEnemy::Process(Float32 _delta)
{
	EAnims currPlayed = static_cast<EAnims>(m_sprite->GetAnim());

	if (m_actor->IsPECollided())// && ANIM_ATTACK_LEFT != currPlayed && ANIM_ATTACK_RIGHT != currPlayed && m_alive && playerAttacking)
	{
		if (ANIM_RUN_LEFT == m_currAnim)
		{
			m_sprite->PlayAnim(ANIM_DEATH_LEFT);
			m_currAnim = ANIM_DEATH_LEFT;
		}
		else if (ANIM_RUN_RIGHT == m_currAnim)
		{
			m_sprite->PlayAnim(ANIM_DEATH_RIGHT);
			m_currAnim = ANIM_DEATH_RIGHT;
		}
		return;
	}
	if (ANIM_DEATH_LEFT == m_currAnim || ANIM_DEATH_RIGHT == m_currAnim)
	{		
		if (ANIM_DEATH_LEFT != currPlayed && ANIM_DEATH_RIGHT != currPlayed)
		{
			m_alive = false;
			m_actor->SetActive(false);
		}
		else
		{
			m_sprite->Process(_delta);
		}
		return;
	}

	if (0 < m_attackDelay) m_attackDelay -= _delta;

	if (m_actor->IsHCollided())
	{
		VECTOR2 vel = m_actor->GetVelocity();
		if (m_left)
		{
			m_left = false;
			if (ANIM_RUN_LEFT == m_currAnim || ANIM_ATTACK_LEFT == m_currAnim)
			{
				if (ANIM_ATTACK_LEFT != m_currAnim)
				{
					m_currAnim = ANIM_RUN_RIGHT;
				}
				m_sprite->SetAnim(ANIM_RUN_RIGHT);
			}
			vel.x = m_moveForce.x;
		}
		else
		{
			m_left = true;
			if (ANIM_RUN_RIGHT == m_currAnim || ANIM_ATTACK_RIGHT == m_currAnim)
			{
				if (ANIM_ATTACK_RIGHT != m_currAnim)
				{
					m_currAnim = ANIM_RUN_LEFT;
				}
				m_sprite->SetAnim(ANIM_RUN_LEFT);
			}
			vel.x = -m_moveForce.x;
		}
		m_actor->SetVelocity(vel);
	}

	switch (m_currAnim)
	{
	case ANIM_RUN_LEFT:
		if (0 >= m_attackDelay)
		{
			m_sprite->PlayAnim(ANIM_ATTACK_LEFT);
			m_currAnim = ANIM_ATTACK_LEFT;
			m_attackDelay = 2.5f;
		}
		break;
	case ANIM_RUN_RIGHT:
		if (0 >= m_attackDelay)
		{
			m_sprite->PlayAnim(ANIM_ATTACK_RIGHT);
			m_currAnim = ANIM_ATTACK_RIGHT;
			m_attackDelay = 2.5f;
		}
		break;
	case ANIM_ATTACK_LEFT:
	case ANIM_ATTACK_RIGHT:
		{
			if (ANIM_ATTACK_LEFT != currPlayed && ANIM_ATTACK_RIGHT != currPlayed)
			{
				m_currAnim = currPlayed;
				m_sprite->SetAnim(m_currAnim);
				//VECTOR2 scale = m_sprite->GetScale();
				//m_actor->SetScale(scale);
			}
		}
		break;
	}
	
	m_pos = m_actor->GetPosition();
	m_sprite->Process(_delta);
}

void CEnemy::Render(VECTOR2 _camPos)
{
	if ((m_pos.x + _camPos.x) > -50 && (m_pos.y - _camPos.y) > -100 &&
		(m_pos.x + _camPos.x) < (m_screenW + 50) && (m_pos.y - _camPos.y) < (m_screenH + 100) && m_alive)
	{
		m_actor->SetActive(true);
		m_sprite->SetPosition(static_cast<Int32>(m_pos.x + _camPos.x), static_cast<Int32>(m_pos.y - _camPos.y));
		m_sprite->Render();
	}
	else if (m_actor->IsActive()) 
	{
		m_actor->SetActive(false);
	}
}

void CEnemy::SetPosition(VECTOR2 _v)
{
	m_actor->SetPosition(_v);
	m_pos = _v;
}

void CEnemy::TriggerAttack(Bool _left)
{
	VECTOR2 vel = m_actor->GetVelocity();

	if (_left)
	{
		m_sprite->PlayAnim(ANIM_ATTACK_LEFT);
		m_currAnim = ANIM_ATTACK_LEFT;
		m_attackDelay = 2.5f;

		if (!m_left)
		{
			m_left = true;
			m_sprite->SetAnim(m_currAnim);
			vel.x = -m_moveForce.x;
			m_actor->SetVelocity(vel);
		}
	}
	else
	{
		m_sprite->PlayAnim(ANIM_ATTACK_RIGHT);
		m_currAnim = ANIM_ATTACK_RIGHT;
		m_attackDelay = 2.5f;

		if (m_left)
		{
			m_left = false;
			m_sprite->SetAnim(m_currAnim);
			vel.x = m_moveForce.x;
			m_actor->SetVelocity(vel);
		}
	}
}