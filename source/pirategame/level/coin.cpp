
// Library Includes

// Local Includes
#include "coin.h"

#include "level.h"

CCoin::CCoin()
	: m_sprite(0)
	, m_actor(0)
	, m_level(0)
	, m_screenW(0)
	, m_screenH(0)
{
}

CCoin::~CCoin()
{
}

Bool CCoin::Initialise(VECTOR2 _pos)
{
	m_pos = _pos;

	if (0 == m_sprite)
	{
		m_sprite = Sprite::CreateSprite("data/art/tilesets/pirateCoin.png", "data/art/tilesets/pirateCoin.xml", true);
		assert(m_sprite);
		m_sprite->AddRef();
	}

	if (0 == m_actor)
	{
		m_actor = Physics::CreateStaticActor(m_pos, m_sprite->GetScale());
		assert(m_actor);
		m_actor->AddRef();
		m_actor->SetType(Physics::EType::TYPE_COIN);
	}
	
	m_screenW = Renderer::activeRenderer->GetWidth();
	m_screenH = Renderer::activeRenderer->GetHeight();
	m_collected = false;

	return true;
}

Bool CCoin::ShutDown()
{
	PY_SAFE_RELEASE(m_sprite);
	Physics::FlushActor(m_actor);
	return true;
}

void CCoin::Process(Float32 _delta)
{
	if (m_actor->IsPCCollided() && !m_collected) 
	{
		m_level->AddToScore(SCORE_VALUE);
		m_collected = true;
		m_actor->SetActive(false);
	}
	else 
	{
		m_sprite->Process(_delta);
	}
}

void CCoin::Render(VECTOR2 _camPos)
{
	if ((m_pos.x + _camPos.x) > -(25.0f * 3.0f) && (m_pos.y - _camPos.y) > -(25.0f * 3.0f) &&
		(m_pos.x + _camPos.x) < (m_screenW + 25.0f * 3.0f) && (m_pos.y - _camPos.y) < (m_screenH + 25.0f * 3.0f) && !m_collected)
	{
		m_actor->SetActive(true);
		m_sprite->SetPosition(static_cast<Int32>(m_pos.x + _camPos.x), static_cast<Int32>(m_pos.y - _camPos.y));
		m_sprite->Render();
	}
	else if (m_actor->IsActive() && !m_collected)
	{
		m_actor->SetActive(false);
	}
}