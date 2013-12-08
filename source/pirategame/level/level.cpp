
// Library Includes

// Local Includes
#include "level.h"

#include "..\character\character.h"
#include "..\character\playable.h"

CLevel::CLevel()
	: m_background(0)
	, m_playable(0)
	, m_platforms(0)
	, m_levelNumber(INVALID_ID)
{

}

CLevel::~CLevel()
{

}

Bool CLevel::Initialise(Int8* _setup)
{
	m_background = Sprite::CreateSprite("data/art/background.png", 0, false);
	assert(m_background);
	m_background->AddRef();
	PY_WRITETOFILE("Background created");

	CREATEPOINTER(m_playable, CPlayable);
	assert(m_playable);
	m_playable->Initialise("data/art/characters/sam/male.png", "data/art/characters/sam/male.xml", "data/xml/characters/sam.xml");
	PY_WRITETOFILE("Animiation created");

	return true;
}

Bool CLevel::ShutDown()
{
	return true;
}

void CLevel::Process(Float32 _delta)
{
	m_playable->Process(_delta);
}

void CLevel::Render()
{
	m_background->Render();

	m_playable->Render();
}