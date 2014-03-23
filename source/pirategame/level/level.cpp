
// Library Includes

// Local Includes
#include "level.h"

#include "..\character\character.h"
#include "..\character\playable.h"
#include "..\character\enemy.h"
#include "tile.h"
#include "platform.h"
#include "parallax.h"

CLevel::CLevel()
	: m_background(0)
	, m_playable(0)
	, m_tiles(0)
	, m_enemies(0)
	, m_levelNumber(INVALID_ID)
	, m_platforms(0)
	, m_numTiles(0)
	, m_numEnemies(0)
	, m_numPlatforms(0)
	, m_screenW(0)
	, m_complete(false)
{
	SDL_memset(m_parallax, 0, sizeof(CParallax*) * 2);
}

CLevel::~CLevel()
{
	
}

Bool CLevel::Initialise(Int8* _setup)
{
	if (0 == m_filePath)
	{
		m_filePath = new Int8[MAX_BUFFER];
		SDL_snprintf(m_filePath, MAX_BUFFER, "%s", _setup);
	}

	m_cameraPos.x = 0.0f;
	m_cameraPos.y = 0.0f;

	// Make platforms
	m_screenW = Renderer::activeRenderer->GetWidth();
	m_screenH = Renderer::activeRenderer->GetHeight();

	FileParser::IParser* setup = FileParser::LoadFile(_setup);
	setup->AddRef();
	VALIDATE(setup->GetValue("tiles", m_numTiles));
	VALIDATE(setup->GetValue("levelNumber", m_levelNumber));
	VALIDATE(setup->GetValue("platforms", m_numPlatforms));
	VALIDATE(setup->GetValue("enemies", m_numEnemies));

	Int8* tileset = 0;
	Int8 path[MAX_BUFFER];
	VALIDATE(setup->GetValue("tileset", &tileset));
	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/background.png", tileset);
	
	if (0 == m_background)
	{
		m_background = Sprite::CreateSprite(path, 0, false);
		assert(m_background);
		m_background->AddRef();
		PY_WRITETOFILE("Background created");
	}
	
	VECTOR2 pos;
		
	UInt32 type = 0;
	Int8 text[MAX_BUFFER];

	if (0 == m_tiles)
	{
		m_tiles = new CTile*[m_numTiles];
		assert(m_tiles);
		SDL_memset(m_tiles, 0, sizeof(CTile*) * m_numTiles);
	
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
	}

	if (0 == m_enemies)
	{
		m_enemies = new CEnemy*[m_numEnemies];
		assert(m_enemies);
		SDL_memset(m_enemies, 0, sizeof(CEnemy*) * m_numEnemies);
	}

	for (Int32 i = 0; i < m_numEnemies; ++i)
	{
		Int8* sprite = 0;
		Int8* spriteSettings = 0;
		Int8* settings = 0;
		SDL_snprintf(text, MAX_BUFFER, "e%i-pos", i + 1);
		VALIDATE(setup->GetValue(text, pos));
		SDL_snprintf(text, MAX_BUFFER, "e%i-sprite", i + 1);
		VALIDATE(setup->GetValue(text, &sprite));		
		SDL_snprintf(text, MAX_BUFFER, "e%i-spriteSettings", i + 1);
		VALIDATE(setup->GetValue(text, &spriteSettings));		
		SDL_snprintf(text, MAX_BUFFER, "e%i-settings", i + 1);
		VALIDATE(setup->GetValue(text, &settings));		

		CREATEPOINTER(m_enemies[i], CEnemy);
		VALIDATE(m_enemies[i]->Initialise(sprite, spriteSettings, settings));
		m_enemies[i]->SetPosition(pos);
		m_enemies[i]->SetPlayer(m_playable);
	}

	if (0 == m_platforms)
	{
		m_platforms = new CPlatform*[m_numPlatforms];
		assert(m_platforms);
		SDL_memset(m_platforms, 0, sizeof(CPlatform*) * m_numPlatforms);
	}

	for (Int32 i = 0; i < m_numPlatforms; ++i)
	{
		CREATEPOINTER(m_platforms[i], CPlatform);
		assert(m_platforms[i]);

		SDL_snprintf(text, MAX_BUFFER, "%iplatform-num", i + 1);

		Int32 number = 0;
		VALIDATE(setup->GetValue(text, number));

		VALIDATE(m_platforms[i]->Initialise(setup, path, number, i + 1));
	}

	if (0 < m_numTiles)
	{
		CREATEPOINTER(m_playable, CPlayable);
		assert(m_playable);
		VALIDATE(m_playable->Initialise("data/art/characters/sam/male.png", "data/art/characters/sam/male.xml", "data/xml/characters/sam.xml"));
		PY_WRITETOFILE("Player created");

		VALIDATE(setup->GetValue("playerStart", pos));
		m_playable->SetPosition(pos);
	}
	
	setup->Release();
	CLEANARRAY(tileset);

	// setup parallax
	CREATEPOINTER(m_parallax[0], CParallax);
	assert(m_parallax[0]);
	m_parallax[0]->Initialise(static_cast<Int32>(LEVEL_WIDTH * 0.3f), static_cast<Int32>(LEVEL_HEIGHT * 0.3f), "");

	CREATEPOINTER(m_parallax[1], CParallax);
	assert(m_parallax[1]);
	m_parallax[1]->Initialise(static_cast<Int32>(LEVEL_WIDTH * 0.6f), static_cast<Int32>(LEVEL_HEIGHT * 0.6f), "");

	// set camera position if player exists
	if (0 == m_playable)
	{
		m_cameraPos.y = static_cast<Float32>(LEVEL_HEIGHT - m_screenH);
		Logger::TrackValue(&m_cameraPos, "Camera Position");
	}

	return true;
}

Bool CLevel::ShutDown()
{
	for (Int16 i = 0; i < 2; ++i)
	{
		m_parallax[i]->ShutDown();
		CLEANDELETE(m_parallax[i]);
	}

	for (Int16 i = 0; i < m_numPlatforms; ++i)
	{
		m_platforms[i]->ShutDown();
		CLEANDELETE(m_platforms[i]);
	}

	for (Int16 i = 0; i < m_numEnemies; ++i)
	{
		m_enemies[i]->ShutDown();
		CLEANDELETE(m_enemies[i]);
	}
	CLEANARRAY(m_enemies);

	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		PY_DELETE_RELEASE(m_tiles[i]);
	}
	CLEANARRAY(m_tiles);

	PY_SAFE_RELEASE(m_background);

	if (0 != m_playable)
	{
		m_playable->ShutDown();
		CLEANDELETE(m_playable);
	}

	Logger::StopTracking("Camera Position");

	return true;
}

Bool CLevel::Reset()
{
	VALIDATE(this->Initialise(m_filePath));
	Logger::WriteToScreen("Level reset");
	return true;
}

void CLevel::Process(Float32 _delta)
{
	if (0 != m_playable)
	{
		m_playable->Process(_delta);

		// camera scrolling
		VECTOR2 pos = m_playable->GetPosition();

		Int32 scrollLeft = static_cast<Int32>(m_screenW * 0.5f);
		Int32 scrollUp = LEVEL_HEIGHT - static_cast<Int32>(m_screenH * 0.5f);

		if (pos.x >= scrollLeft && pos.x + scrollLeft < LEVEL_WIDTH)
		{
			m_cameraPos.x = -(pos.x - scrollLeft); 
		}
		if (pos.y < scrollUp)
		{
			m_cameraPos.y = (LEVEL_HEIGHT - m_screenH) - (scrollUp - pos.y);
			if (m_cameraPos.y < 0.0f)
			{
				m_cameraPos.y = 0.0f;
			}
		}
		else
		{
			m_cameraPos.y = static_cast<Float32>(LEVEL_HEIGHT - m_screenH);
		}

		Physics::camPosition = m_cameraPos;

		// check player position for end of level
		if (pos.x >= (LEVEL_WIDTH - 200))
		{
			m_complete = true;
		}
	}

	for (Int16 i = 0; i < m_numPlatforms; ++i)
	{
		if (m_platforms[i]->IsActive())
		{
			m_platforms[i]->Process(_delta);
		}
	}

	for (Int16 i = 0; i < m_numEnemies; ++i)
	{
		if (m_enemies[i]->IsActive())
		{
			m_enemies[i]->Process(_delta);
		}
	}

	for (Int16 i = 0; i < 2; ++i)
	{
		m_parallax[i]->Process(_delta);
	}
}

void CLevel::Render()
{
	m_background->Render();

	// Render parallax layers
	m_parallax[1]->Render(m_cameraPos);
	m_parallax[0]->Render(m_cameraPos);

	// Render tiles
	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		m_tiles[i]->Render(m_cameraPos);
	}
	
	// Render platforms
	for (Int16 i = 0; i < m_numPlatforms; ++i)
	{
		m_platforms[i]->Render(m_cameraPos);
	}

	// Render Enemies
	for (Int16 i = 0; i < m_numEnemies; ++i) 
	{
		m_enemies[i]->Render(m_cameraPos);
	}

	// Render player
	if (0 != m_playable)
	{
		m_playable->Render(m_cameraPos);
	}
}