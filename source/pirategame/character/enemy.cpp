// Library Includes

// Local Includes
#include "enemy.h"

CEnemy::CEnemy()
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

	m_currAnim = ANIM_RUN_LEFT;
	m_sprite = Sprite::CreateSprite(_spriteSheet, _spriteInfo, true);
	assert(m_sprite);
	m_sprite->AddRef();
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

	m_actor = Physics::CreateDynamicActor(max, maxA, m_pos, m_sprite->GetScale(), mass, static_cast<Physics::EType>(type));
	assert(m_actor);
	m_actor->AddRef();
	m_actor->SetActive(true);
	m_actor->SetVCollided(true);
	settings->Release();

	return true;
}

Bool CEnemy::ShutDown()
{
	PY_SAFE_RELEASE(m_sprite);
	PY_SAFE_RELEASE(m_actor);
	return true;
}

void CEnemy::Process(Float32 _delta)
{
	m_pos = m_actor->GetPosition();
	m_sprite->Process(_delta);
}

void CEnemy::Render(VECTOR2 _camPos)
{
	m_sprite->SetPosition(static_cast<Int32>(m_pos.x + _camPos.x), static_cast<Int32>(m_pos.y + _camPos.y));
	m_sprite->Render();
}

void CEnemy::SetPosition(VECTOR2 _v)
{
	m_actor->SetPosition(_v);
	m_pos = _v;
}