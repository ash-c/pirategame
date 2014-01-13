
// Library Includes

// Local Includes
#include "level.h"

#include "tile.h"
#include "platform.h"

CLevel::CLevel()
	: m_background(0)
	, m_tiles(0)
	, m_levelNumber(INVALID_ID)
	, m_platforms(0)
	, m_numPlatforms(0)
{

}

CLevel::~CLevel()
{
	
}

Bool CLevel::Initialise(Int8* _setup)
{
	// Make platforms
	Int32 width = Renderer::activeRenderer->GetWidth();

	FileParser::IParser* setup = FileParser::LoadFile(_setup);
	setup->AddRef();
	VALIDATE(setup->GetValue("tiles", m_numTiles));
	VALIDATE(setup->GetValue("levelNumber", m_levelNumber));
	VALIDATE(setup->GetValue("platforms", m_numPlatforms));

	Int8* tileset = 0;
	Int8 path[MAX_BUFFER];
	VALIDATE(setup->GetValue("tileset", &tileset));
	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/background.png", tileset);
	
	m_background = Sprite::CreateSprite(path, 0, false);
	assert(m_background);
	m_background->AddRef();
	PY_WRITETOFILE("Background created");
	
	VECTOR2 pos;

	m_tiles = new CTile*[m_numTiles];
	assert(m_tiles);
	SDL_memset(m_tiles, 0, sizeof(CTile*) * m_numTiles);
	
	UInt32 type = 0;
	Int8 text[MAX_BUFFER];
	
	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/tiles.png", tileset);

	for (Int32 i = 0; i < m_numTiles; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "%i-pos", i + 1);
		VALIDATE(setup->GetValue(text, pos));

		SDL_snprintf(text, MAX_BUFFER, "%i-type", i + 1);
		VALIDATE(setup->GetValue(text, type));

		CREATEPOINTER(m_tiles[i], CTile);
		VALIDATE(m_tiles[i]->Initialise(path, pos, static_cast<ETileType>(type)));
		m_tiles[i]->AddRef();
	}

	m_platforms = new CPlatform*[m_numPlatforms];
	assert(m_platforms);
	SDL_memset(m_platforms, 0, sizeof(CPlatform*) * m_numPlatforms);

	for (Int32 i = 0; i < m_numPlatforms; ++i)
	{
		CREATEPOINTER(m_platforms[i], CPlatform);
		assert(m_platforms[i]);

		SDL_snprintf(text, MAX_BUFFER, "%iplatform-num", i + 1);

		Int32 number = 0;
		VALIDATE(setup->GetValue(text, number));

		VALIDATE(m_platforms[i]->Initialise(setup, path, number, i + 1));
	}

	setup->Release();

	CLEANARRAY(tileset);
	return true;
}

Bool CLevel::ShutDown()
{
	for (Int16 i = 0; i < m_numPlatforms; ++i)
	{
		m_platforms[i]->ShutDown();
		CLEANDELETE(m_platforms[i]);
	}

	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		PY_DELETE_RELEASE(m_tiles[i]);
	}

	CLEANARRAY(m_tiles);

	PY_SAFE_RELEASE(m_background);

	return true;
}

void CLevel::Process(Float32 _delta)
{
	for (Int16 i = 0; i < m_numPlatforms; ++i)
	{
		m_platforms[i]->Process(_delta);
	}
}

void CLevel::Render()
{
	m_background->Render();

	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		m_tiles[i]->Render();
	}

	for (Int16 i = 0; i < m_numPlatforms; ++i)
	{
		m_platforms[i]->Render();
	}
}