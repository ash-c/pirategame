
// Library Includes

// Local Includes
#include "level.h"

#include "..\character\character.h"
#include "..\character\playable.h"
#include "..\character\enemy.h"
#include "tile.h"
#include "water.h"
#include "coin.h"
#include "platform.h"
#include "parallax.h"

CLevel::CLevel()
	: m_background(0)
	, m_numbers(0)
	, m_playable(0)
	, m_tiles(0)
	, m_water(0)
	, m_coins(0)
	, m_enemies(0)
	, m_platforms(0)
	, m_parallax(0)
	, m_numTiles(0)
	, m_numWater(0)
	, m_numEnemies(0)
	, m_numPlatforms(0)
	, m_numCoins(0)
	, m_paraCount(0)
	, m_screenW(0)
	, m_score(0)
	, m_levelNum(0)
	, m_complete(false)
{
}

CLevel::~CLevel()
{
	
}

Bool CLevel::Initialise(Int8* _setup, Int32 _num)
{
	if (0 == m_filePath)
	{
		m_filePath = new Int8[MAX_BUFFER];
		SDL_snprintf(m_filePath, MAX_BUFFER, "%s", _setup);
	}

	m_cameraPos.x = 0.0f;
	m_cameraPos.y = 0.0f;
	m_score = 0;
	m_levelNum = _num;

	Logger::TrackValue(&m_score, "Level Score");

	// Make platforms
	m_screenW = Renderer::activeRenderer->GetWidth();
	m_screenH = Renderer::activeRenderer->GetHeight();

	FileParser::IParser* setup = FileParser::LoadFile(_setup);
	setup->Load();
	setup->AddRef();
	VALIDATE(setup->GetValue("tiles", m_numTiles));
	VALIDATE(setup->GetValue("platforms", m_numPlatforms));
	VALIDATE(setup->GetValue("enemies", m_numEnemies));
	VALIDATE(setup->GetValue("water", m_numWater));
	VALIDATE(setup->GetValue("coins", m_numCoins));
	VALIDATE(setup->GetValue("bkgMusic", &m_bkgMusic));

	Int8* tileset = 0;
	Int8 path[MAX_BUFFER];
	VALIDATE(setup->GetValue("tileset", &tileset));
	
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

	if (0 == m_water)
	{
		m_water = new CWater*[m_numWater];
		assert(m_water);
		SDL_memset(m_water, 0, sizeof(CWater*) * m_numWater);
	
		SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/water.png", tileset);

		for (Int32 i = 0; i < m_numWater; ++i)
		{
			SDL_snprintf(text, MAX_BUFFER, "%iw-pos", i + 1);
			VALIDATE(setup->GetValue(text, pos));

			SDL_snprintf(text, MAX_BUFFER, "%iw-type", i + 1);
			VALIDATE(setup->GetValue(text, type));

			CREATEPOINTER(m_water[i], CWater);
			VALIDATE(m_water[i]->Initialise(path, pos, static_cast<ETileType>(type)));
			m_water[i]->AddRef();
		}
	}

	if (0 == m_coins)
	{
		m_coins = new CCoin*[m_numCoins];
		assert(m_coins);
		SDL_memset(m_coins, 0, sizeof(CCoin*) * m_numCoins);
	}

	for (Int32 i = 0; i < m_numCoins; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "c%i-pos", i + 1);
		VALIDATE(setup->GetValue(text, pos));
		CREATEPOINTER(m_coins[i], CCoin);
		VALIDATE(m_coins[i]->Initialise(pos));
		m_coins[i]->SetLevel(this);
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
		m_enemies[i]->SetLevel(this);
	}

	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/tiles.png", tileset);
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
	VALIDATE(setup->ShutDown());
	setup = 0;

	// setup parallax
	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/tileset.ini", tileset);
	setup = FileParser::LoadFile(path);
	assert(setup);
	setup->AddRef();

	Int8* background = 0;
	VALIDATE(setup->GetValue("static", &background, "parallax"));
	assert(background);

	if (0 == m_background)
	{
		m_background = Sprite::CreateSprite(background, 0, false);
		assert(m_background);
		m_background->AddRef();
		PY_WRITETOFILE("Background created");
	}

	setup->Release();
	CLEANARRAY(tileset);

	VALIDATE(setup->GetValue("count", m_paraCount, "parallax"));

	if (0 < m_paraCount && 0 == m_parallax)
	{
		m_parallax = new CParallax*[m_paraCount];
		assert(m_parallax);
		SDL_memset(m_parallax, 0, sizeof(CParallax*) * m_paraCount);
	}

	for (Int16 i = 0; i < m_paraCount; ++i)
	{
		CREATEPOINTER(m_parallax[i], CParallax);
		assert(m_parallax[i]);
		Int8 temp[MAX_BUFFER];
		SDL_snprintf(temp, MAX_BUFFER, "%i", i + 1);
		VALIDATE(m_parallax[i]->Initialise(setup, temp));
	}
	
	// set camera position if player exists
	if (0 == m_playable)
	{
		m_cameraPos.y = static_cast<Float32>(LEVEL_HEIGHT - m_screenH);
		Logger::TrackValue(&m_cameraPos, "Camera Position");
	}

	// setup number display
	m_numberScale.x = 13.0f;
	m_numberScale.y = 20.0f;
	Int32 x = 0;
	for (Int16 i = 0; i < 10; ++i)
	{
		m_numberClips[i].y = 0;
		m_numberClips[i].w = static_cast<Int32>(m_numberScale.x);
		m_numberClips[i].h = static_cast<Int32>(m_numberScale.y);
		m_numberClips[i].x = x;
		x += static_cast<Int32>(m_numberScale.x);
	}

	m_numbers = Sprite::CreateSprite("data/art/tilesets/numbers.png", 0, false);
	assert(m_numbers);
	m_numberPos.x = m_screenW - 50.0f;
	m_numberPos.y =  50.0f;
	m_numbers->SetClip(&m_numberClips[0]);
	m_numbers->SetScale(static_cast<Int32>(m_numberScale.x), static_cast<Int32>(m_numberScale.y));

	Sound::PlayBkgMusic(m_bkgMusic);

	return true;
}

Bool CLevel::ShutDown()
{
	Logger::StopTracking("Level Score");

	if (0 != m_parallax)
	{
		for (Int16 i = 0; i < m_paraCount; ++i)
		{
			m_parallax[i]->ShutDown();
			CLEANDELETE(m_parallax[i]);
		}
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

	for (Int16 i = 0; i < m_numCoins; ++i)
	{
		m_coins[i]->ShutDown();
		CLEANDELETE(m_coins[i]);
	}
	CLEANARRAY(m_coins);

	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		PY_DELETE_RELEASE(m_tiles[i]);
	}
	CLEANARRAY(m_tiles);

	for (Int16 i = 0; i < m_numWater; ++i)
	{
		PY_DELETE_RELEASE(m_water[i]);
	}
	CLEANARRAY(m_water);

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
	VALIDATE(this->Initialise(m_filePath, m_levelNum));
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

	for (Int16 i = 0; i < m_numCoins; ++i)
	{
		if (m_coins[i]->IsActive())
		{
			m_coins[i]->Process(_delta);
		}
	}

	if (0 != m_parallax)
	{
		for (Int16 i = 0; i < m_paraCount; ++i)
		{
			m_parallax[i]->Process(_delta);
		}
	}
}

void CLevel::Render()
{
	m_background->Render();

	// Render parallax layers
	if (0 != m_parallax)
	{
		for (Int16 i = 0; i < m_paraCount; ++i)
		{
			m_parallax[i]->Render(m_cameraPos);
		}
	}	

	// Render water
	for (Int16 i = 0; i < m_numWater; ++i)
	{
		m_water[i]->Render(m_cameraPos);
	}

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

	// Render Coins
	for (Int16 i = 0; i < m_numCoins; ++i) 
	{
		m_coins[i]->Render(m_cameraPos);
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

	// Render score
	Int32 temp = m_score;
	Float32 x = m_numberPos.x;
	if (0 != m_playable)
	{
		if (0 == temp)
		{
			m_numbers->SetClip(&m_numberClips[0]);
			m_numbers->SetPosition(static_cast<Int32>(x), static_cast<Int32>(m_numberPos.y));
			m_numbers->Render();
			x -= m_numberScale.x;
		}

		while (temp > 0) // rendering the individual digits
		{
			Int32 index = temp % 10;
			m_numbers->SetClip(&m_numberClips[index]);
			m_numbers->SetPosition(static_cast<Int32>(x), static_cast<Int32>(m_numberPos.y));
			m_numbers->Render();
			x -= m_numberScale.x;

			temp /= 10;
		}
	}
}

Bool CLevel::IsNext(VECTOR2 _pos)
{
	for (Int32 i = 0; i < m_numTiles; ++i)
	{
		VECTOR2 tilePos = m_tiles[i]->GetPos();
		if (_pos.x < tilePos.x + TILE_WIDTH * 0.5f && 
			_pos.x > tilePos.x - TILE_WIDTH * 0.5f &&
			_pos.y + TILE_HEIGHT * 0.5f > tilePos.y &&
			_pos.y < tilePos.y)
		{
			return true;
		}
	}

	return false;
}