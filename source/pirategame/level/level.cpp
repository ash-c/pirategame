
// Library Includes

// Local Includes
#include "level.h"

#include "..\character\character.h"
#include "..\character\playable.h"
#include "tile.h"

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
	PY_WRITETOFILE("Animation created");

	// Make platforms
	Int32 width = Renderer::activeRenderer->GetWidth();

	FileParser::IParser* setup = FileParser::LoadFile(_setup);
	setup->AddRef();
	VALIDATE(setup->GetValue("tiles", m_numTiles));

	m_platforms = new CTile*[m_numTiles];
	assert(m_platforms);
	SDL_memset(m_platforms, 0, sizeof(CTile*) * m_numTiles);
	
	UInt32 type = 0;
	VECTOR2 pos;
	Int8 text[MAX_BUFFER];

	for (Int32 i = 0; i < m_numTiles; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "%i-pos", i + 1);
		VALIDATE(setup->GetValue(text, pos));

		SDL_snprintf(text, MAX_BUFFER, "%i-type", i + 1);
		VALIDATE(setup->GetValue(text, type));

		CREATEPOINTER(m_platforms[i], CTile);
		VALIDATE(m_platforms[i]->Initialise("data/art/levels/tiles.png", pos, static_cast<ETileType>(type)));
		m_platforms[i]->AddRef();
	}

	VALIDATE(setup->GetValue("playerStart", pos));
	m_playable->SetPosition(pos);

	setup->Release();
	return true;
}

Bool CLevel::ShutDown()
{
	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		PY_DELETE_RELEASE(m_platforms[i]);
	}

	CLEANARRAY(m_platforms);

	PY_SAFE_RELEASE(m_background);

	m_playable->ShutDown();
	CLEANDELETE(m_playable);

	return true;
}

void CLevel::Process(Float32 _delta)
{
	m_playable->Process(_delta);
}

void CLevel::Render()
{
	m_background->Render();

	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		m_platforms[i]->Render();
	}

	m_playable->Render();
}