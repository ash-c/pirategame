
// Library Includes

// Local Includes
#include "level.h"

#include "tile.h"
#include "water.h"
#include "platform.h"
#include "../../../pirategame/level/coin.h"
#include "../../../pirategame/character/enemy.h"
#include "../../../pirategame/character/playable.h"

#include "../../parser/parser.h"

using namespace Papyrus;

CLevel::CLevel()
	: m_surface(0)
	, m_grid(0)
	, m_background(0)
	, m_playable(0)
	, m_tiles(0)
	, m_platforms(0)
	, m_gridRects(0)
	, m_numTiles(0)
	, m_numWater(0)
	, m_numQuicksand(0)
	, m_numPlatforms(0)
	, m_numEnemies(0)
	, m_numRects(0)
	, m_numCoins(0)
	, m_tileset(0)
{

}

CLevel::~CLevel()
{
	
}

Bool CLevel::Initialise(Int8* _setup)
{
	UInt16 len = SDL_strlen(_setup);

	if (len > 0)
	{
		m_filePath = new Int8[len + 1];
		SDL_strlcpy(m_filePath, _setup, len + 1);
	}

	// Make platforms
	Int32 width = Renderer::activeRenderer->GetWidth();

	// Fill grid with rectangles.
	m_numRects = LEVEL_WIDTH/TILE_WIDTH * LEVEL_HEIGHT/TILE_WIDTH;

	m_gridRects = new SDL_Rect[m_numRects];

	m_gridRects[0].x = 0;
	m_gridRects[0].y = 0;
	m_gridRects[0].w = TILE_WIDTH;
	m_gridRects[0].h = TILE_HEIGHT;

	for (Int32 i = 1; i < m_numRects; ++i)
	{
		m_gridRects[i].x = m_gridRects[i - 1].x;
		m_gridRects[i].y = m_gridRects[i - 1].y;
		m_gridRects[i].w = TILE_WIDTH;
		m_gridRects[i].h = TILE_HEIGHT;

		m_gridRects[i].x += TILE_WIDTH;

		if (m_gridRects[i].x >= (LEVEL_WIDTH))
		{
			m_gridRects[i].x = 0;
			m_gridRects[i].y += TILE_HEIGHT;
		}
	}

	VALIDATE(Renderer::activeRenderer->LoadTexture("data/art/editor/grid.png", &m_grid));
	assert(m_grid);

	FileParser::IParser* setup = FileParser::LoadFile(_setup);
	setup->AddRef();
	Int32 tiles = 0;
	Int32 water = 0;
	Int32 quicksand = 0;
	Int32 coins = 0;
	VALIDATE(setup->GetValue("tiles", tiles));
	VALIDATE(setup->GetValue("platforms", m_numPlatforms));
	VALIDATE(setup->GetValue("enemies", m_numEnemies));
	VALIDATE(setup->GetValue("water", water));
	VALIDATE(setup->GetValue("quicksand", quicksand));	
	VALIDATE(setup->GetValue("coins", coins));	

	Int8 path[MAX_BUFFER];
	VALIDATE(setup->GetValue("tileset", &m_tileset));
	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/background.png", m_tileset);
	
	m_background = Sprite::CreateSprite(path, 0, false);
	assert(m_background);
	m_background->AddRef();
	PY_WRITETOFILE("Background created");
	
	VECTOR2 pos;

	Bool playerExists = setup->GetValue("playerStart", pos);
	if (playerExists) 
	{
		if (pos.x != 0 && pos.y != 0)
		{
			CREATEPOINTER(m_playable, CPlayable);
			assert(m_playable);
			VALIDATE(m_playable->Initialise("data/art/characters/sam/male.png", "data/art/characters/sam/male.xml", "data/xml/characters/sam.xml"));
			m_playable->SetPosition(pos);
		}
	}
	
	UInt32 type = 0;
	Int8 text[MAX_BUFFER];
	
	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/tiles.png", m_tileset);

	for (Int32 i = 0; i < tiles; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "%i-pos", i + 1);
		VALIDATE(setup->GetValue(text, pos));
		AddTile(pos);
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

		CEnemy* temp = 0;
		CREATEPOINTER(temp, CEnemy);
		VALIDATE(temp->Initialise(sprite, spriteSettings, settings));
		temp->SetPosition(pos);
		m_enemies.push_back(temp);
		CLEANARRAY(sprite);
		CLEANARRAY(spriteSettings);
		CLEANARRAY(settings);
	}

	for (Int32 i = 0; i < m_numPlatforms; ++i)
	{
		CPlatform* temp = 0;
		CREATEPOINTER(temp, CPlatform);
		assert(temp);
		m_platforms.push_back(temp);

		SDL_snprintf(text, MAX_BUFFER, "%iplatform-num", i + 1);

		Int32 number = 0;
		VALIDATE(setup->GetValue(text, number));

		VALIDATE(m_platforms[i]->Initialise(setup, path, number, i + 1));
	}

	for (Int32 i = 0; i < coins; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "c%i-pos", i + 1);
		VALIDATE(setup->GetValue(text, pos));
		AddCoin(pos);
	}
	
	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/water.png", m_tileset);
	for (Int32 i = 0; i < water; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "%iw-pos", i + 1);
		setup->GetValue(text, pos);
		AddWater(pos);
	}

	setup->Release();

	Logger::TrackValue(&m_cameraPos, "Camera Position");

	return true;
}

Bool CLevel::ShutDown()
{
	for (UInt16 i = 0; i < m_platforms.size(); ++i)
	{
		m_platforms[i]->ShutDown();
		CLEANDELETE(m_platforms[i]);
	}
	m_platforms.clear();

	// Need to release and shutdown all before deleting because of the tiles knowing of tiles nearby.
	for (UInt16 i = 0; i < m_tiles.size(); ++i)
	{
		m_tiles[i]->Release();
		m_tiles[i]->ShutDown();
	}

	for (UInt16 i = 0; i < m_water.size(); ++i)
	{
		m_water[i]->Release();
		m_water[i]->ShutDown();
	}

	for (UInt16 i = 0; i < m_enemies.size(); ++i)
	{
		m_enemies[i]->ShutDown();
	}

	for (UInt16 i = 0; i < m_tiles.size(); ++i)
	{
		CLEANDELETE(m_tiles[i]);
	}
	m_tiles.clear();

	for (UInt16 i = 0; i < m_water.size(); ++i)
	{
		CLEANDELETE(m_water[i]);
	}
	m_water.clear();

	for (UInt16 i = 0; i < m_enemies.size(); ++i)
	{
		CLEANDELETE(m_enemies[i]);
	}
	m_enemies.clear();

	for (UInt16 i = 0; i < m_coins.size(); ++i)
	{
		m_coins[i]->ShutDown();
		CLEANDELETE(m_coins[i]);
	}
	m_coins.clear();

	PY_SAFE_RELEASE(m_background);

	CLEANARRAY(m_tileset);

	SDL_DestroyTexture(m_grid);

	if (0 != m_playable) m_playable->ShutDown();
	CLEANDELETE(m_playable);

	Logger::StopTracking("Camera Position");

	return true;
}

void CLevel::Process(Float32 _delta)
{
}

void CLevel::Render()
{
	m_background->Render();

	// Render grid
	SDL_Rect rectPos;
	rectPos.x = static_cast<Int32>(m_cameraPos.x);
	rectPos.y = static_cast<Int32>((LEVEL_HEIGHT - TILE_HEIGHT) - m_cameraPos.y);//(Renderer::activeRenderer->GetHeight() - TILE_HEIGHT + m_cameraPos.y);
	rectPos.w = TILE_WIDTH;
	rectPos.h = TILE_HEIGHT;

	for (Int32 i = 0; i < m_numRects; ++i)
	{
		Renderer::activeRenderer->Render(m_grid, &rectPos, NULL);
		rectPos.x += TILE_WIDTH;

		if (rectPos.x >= (LEVEL_WIDTH + m_cameraPos.x))
		{
			rectPos.x = static_cast<Int32>(m_cameraPos.x);
			rectPos.y -= TILE_HEIGHT;
		}
	}

	// Render Water	
	for (UInt16 i = 0; i < m_water.size(); ++i)
	{
		m_water[i]->Render(m_cameraPos);
	}

	// Render tiles
	for (UInt16 i = 0; i < m_tiles.size(); ++i)
	{
		m_tiles[i]->Render(m_cameraPos);
	}

	// Render platforms
	for (UInt16 i = 0; i < m_platforms.size(); ++i)
	{
		m_platforms[i]->Render(m_cameraPos);
	}

	// Render Coins
	for (UInt16 i = 0; i < m_coins.size(); ++i) 
	{
		m_coins[i]->Render(m_cameraPos);
	}

	// Render Enemies
	for (UInt16 i = 0; i < m_enemies.size(); ++i) 
	{
		m_enemies[i]->Render(m_cameraPos);
	}

	// Render player start
	if (0 != m_playable) m_playable->Render(m_cameraPos);
}

Bool CLevel::Save(Int8* _path)
{
	FileParser::IParser* save = 0;
	if (0 == _path && 0 != m_filePath)
	{
		save = FileParser::LoadFile(m_filePath);
	}
	else if (0 != _path)
	{
		save = FileParser::LoadFile(_path);
	}
	else
	{
		Logger::Write("No valid file path");
		return false;
	}
	assert(save);
	save->AddRef();
	
	if (0 != m_playable)
	{
		VALIDATE(save->AddValue("playerStart", m_playable->GetPosition()));
	}
	VALIDATE(save->AddValue("tileset", m_tileset));
	VALIDATE(save->AddValue("tiles", m_numTiles));
	VALIDATE(save->AddValue("water", m_numWater));
	VALIDATE(save->AddValue("quicksand", m_numQuicksand));
	VALIDATE(save->AddValue("platforms", m_numPlatforms));
	VALIDATE(save->AddValue("enemies", m_numEnemies));
	VALIDATE(save->AddValue("coins", m_numCoins));
#pragma todo("this needs to save a different bkg music path based on the tileset")
	VALIDATE(save->AddValue("bkgMusic", "data/audio/music/shipBeach.wav"));

	Int8 text[MAX_BUFFER];
	for (UInt16 i = 0; i < m_numTiles; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "%i-pos", i + 1);
		VALIDATE(save->AddValue(text, m_tiles[i]->GetPos()));
		SDL_snprintf(text, MAX_BUFFER, "%i-type", i + 1);
		VALIDATE(save->AddValue(text, m_tiles[i]->GetType()));
	}

	for (UInt16 i = 0; i < m_numWater; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "%iw-pos", i + 1);
		VALIDATE(save->AddValue(text, m_water[i]->GetPos()));
		SDL_snprintf(text, MAX_BUFFER, "%iw-type", i + 1);
		VALIDATE(save->AddValue(text, m_water[i]->GetType()));
	}

	for (UInt16 i = 0; i < m_numPlatforms; ++i)
	{
		if (!m_platforms[i]->Save(save, i + 1))
		{
			Logger::Write("Saving platform number %i failed", i + 1);
		}
	}

	for (UInt16 i = 0; i < m_numCoins; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "c%i-pos", i + 1);
		VALIDATE(save->AddValue(text, m_coins[i]->GetPos()));
	}

	for (UInt16 i = 0; i < m_numEnemies; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "e%i-pos", i + 1);
		VALIDATE(save->AddValue(text, m_enemies[i]->GetPosition()));
		SDL_snprintf(text, MAX_BUFFER, "e%i-sprite", i + 1);
		VALIDATE(save->AddValue(text, m_enemies[i]->m_spriteSheet));
		SDL_snprintf(text, MAX_BUFFER, "e%i-spriteSettings", i + 1);
		VALIDATE(save->AddValue(text, m_enemies[i]->m_spriteSettings));
		SDL_snprintf(text, MAX_BUFFER, "e%i-settings", i + 1);
		VALIDATE(save->AddValue(text, m_enemies[i]->m_settings));
	}

	VALIDATE(save->Save());
	save->Release();

	return true;
}

void CLevel::CameraPos(VECTOR2 _pos)
{
	m_cameraPos = _pos;
}

Bool CLevel::AddTile(VECTOR2 _pos)
{
	CheckAgainstGrid(&_pos);

	// check for pre existing
	if (CheckForExistingTile(&_pos)) return false;

	// Calculate right type of tile.
	ETileType type = TYPE_ALONE;
	CTile* temp = 0;
	CREATEPOINTER(temp, CTile);
	
	Int8 path[MAX_BUFFER];
	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/tiles.png", m_tileset);
	VALIDATE(temp->Initialise(path, _pos, type));
	temp->AddRef();

	m_tiles.push_back(temp);
	++m_numTiles;

	VECTOR2 surr[8]; // 0-left, 1-top left, 2-top, 3-top right, 4-right, 5-bottom right, 6-bottom, 7-bottom left

	for (UInt16 i = 0; i < 8; ++i) { surr[i] = _pos; }

	surr[0].x -= TILE_WIDTH;
	surr[1].x -= TILE_WIDTH;
	surr[1].y -= TILE_HEIGHT;
	surr[2].y -= TILE_HEIGHT;
	surr[3].x += TILE_WIDTH;
	surr[3].y -= TILE_HEIGHT;
	surr[4].x += TILE_WIDTH;
	surr[5].x += TILE_WIDTH;
	surr[5].y += TILE_HEIGHT;
	surr[6].y += TILE_HEIGHT;
	surr[7].x -= TILE_WIDTH;
	surr[7].y += TILE_HEIGHT;

	for (UInt16 i = 0; i < m_tiles.size(); ++i)
	{
		VECTOR2 pos = m_tiles[i]->GetPos();
		// left
		if (surr[0].x == pos.x && surr[0].y == pos.y)
		{
			temp->AddLinked(m_tiles[i]);
			m_tiles[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// top left
		if (surr[1].x == pos.x && surr[1].y == pos.y)
		{
			temp->AddLinked(m_tiles[i]);
			m_tiles[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// top
		if (surr[2].x == pos.x && surr[2].y == pos.y)
		{
			temp->AddLinked(m_tiles[i]);
			m_tiles[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// top right
		if (surr[3].x == pos.x && surr[3].y == pos.y)
		{
			temp->AddLinked(m_tiles[i]);
			m_tiles[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// right
		if (surr[4].x == pos.x && surr[4].y == pos.y)
		{
			temp->AddLinked(m_tiles[i]);
			m_tiles[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// bottom right
		if (surr[5].x == pos.x && surr[5].y == pos.y)
		{
			temp->AddLinked(m_tiles[i]);
			m_tiles[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// bottom
		if (surr[6].x == pos.x && surr[6].y == pos.y)
		{
			temp->AddLinked(m_tiles[i]);
			m_tiles[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// bottom left
		if (surr[7].x == pos.x && surr[7].y == pos.y)
		{
			temp->AddLinked(m_tiles[i]);
			m_tiles[i]->AddLinked(temp);
			temp->UpdateType();
		}
	}
	return true;
}

Bool CLevel::RemoveTile(VECTOR2 _pos)
{	
	CheckAgainstGrid(&_pos);
	
	// check for existing
	for (Int32 i = 0; i < m_numTiles; ++i)
	{
		if (m_tiles[i]->GetPos().x == _pos.x &&
			m_tiles[i]->GetPos().y == _pos.y)
		{
			// Tile exists, remove
			PY_DELETE_RELEASE(m_tiles[i]);
			m_tiles.erase(m_tiles.begin() + i);
			--m_numTiles;

			return true;
		}
	}

	// Tile doesn't exist
	return false;
}

Bool CLevel::AddCoin(VECTOR2 _pos)
{
	//CheckAgainstGrid(&_pos);

	CCoin* temp = 0;
	CREATEPOINTER(temp, CCoin);
	
	VALIDATE(temp->Initialise(_pos));

	m_coins.push_back(temp);
	++m_numCoins;

	return true;
}

Bool CLevel::RemoveCoin(VECTOR2 _pos)
{
	//CheckAgainstGrid(&_pos);

	// check for existing
	for (Int32 i = 0; i < m_numCoins; ++i)
	{
		if (m_coins[i]->GetPos().x <= _pos.x + 15 &&
			m_coins[i]->GetPos().y <= _pos.y + 15 &&
			m_coins[i]->GetPos().x >= _pos.x - 15 &&
			m_coins[i]->GetPos().y >= _pos.y - 15)
		{
			// Coin exists, remove
			m_coins[i]->ShutDown();
			CLEANDELETE(m_coins[i]);
			m_coins.erase(m_coins.begin() + i);
			--m_numCoins;

			return true;
		}
	}

	// Tile doesn't exist
	return false;
}

Bool CLevel::AddWater(VECTOR2 _pos)
{
	CheckAgainstGrid(&_pos);

	// check for pre existing
	if (CheckForExistingTile(&_pos)) return false;

	// Calculate right type of tile.
	ETileType type = TYPE_ALONE;
	CWater* temp = 0;
	CREATEPOINTER(temp, CWater);
	
	Int8 path[MAX_BUFFER];
	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/water.png", m_tileset);
	VALIDATE(temp->Initialise(path, _pos, type));
	temp->AddRef();

	m_water.push_back(temp);
	++m_numWater;

	VECTOR2 surr[8]; // 0-left, 1-top left, 2-top, 3-top right, 4-right, 5-bottom right, 6-bottom, 7-bottom left

	for (UInt16 i = 0; i < 8; ++i) { surr[i] = _pos; }

	surr[0].x -= TILE_WIDTH;
	surr[1].x -= TILE_WIDTH;
	surr[1].y -= TILE_HEIGHT;
	surr[2].y -= TILE_HEIGHT;
	surr[3].x += TILE_WIDTH;
	surr[3].y -= TILE_HEIGHT;
	surr[4].x += TILE_WIDTH;
	surr[5].x += TILE_WIDTH;
	surr[5].y += TILE_HEIGHT;
	surr[6].y += TILE_HEIGHT;
	surr[7].x -= TILE_WIDTH;
	surr[7].y += TILE_HEIGHT;

	for (UInt16 i = 0; i < m_water.size(); ++i)
	{
		VECTOR2 pos = m_water[i]->GetPos();
		// left
		if (surr[0].x == pos.x && surr[0].y == pos.y)
		{
			temp->AddLinked(m_water[i]);
			m_water[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// top left
		if (surr[1].x == pos.x && surr[1].y == pos.y)
		{
			temp->AddLinked(m_water[i]);
			m_water[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// top
		if (surr[2].x == pos.x && surr[2].y == pos.y)
		{
			temp->AddLinked(m_water[i]);
			m_water[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// top right
		if (surr[3].x == pos.x && surr[3].y == pos.y)
		{
			temp->AddLinked(m_water[i]);
			m_water[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// right
		if (surr[4].x == pos.x && surr[4].y == pos.y)
		{
			temp->AddLinked(m_water[i]);
			m_water[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// bottom right
		if (surr[5].x == pos.x && surr[5].y == pos.y)
		{
			temp->AddLinked(m_water[i]);
			m_water[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// bottom
		if (surr[6].x == pos.x && surr[6].y == pos.y)
		{
			temp->AddLinked(m_water[i]);
			m_water[i]->AddLinked(temp);
			temp->UpdateType();
		}
		// bottom left
		if (surr[7].x == pos.x && surr[7].y == pos.y)
		{
			temp->AddLinked(m_water[i]);
			m_water[i]->AddLinked(temp);
			temp->UpdateType();
		}
	}
	return true;
}

Bool CLevel::RemoveWater(VECTOR2 _pos)
{
	CheckAgainstGrid(&_pos);
	
	// check for existing
	for (Int32 i = 0; i < m_numWater; ++i)
	{
		if (m_water[i]->GetPos().x == _pos.x &&
			m_water[i]->GetPos().y == _pos.y)
		{
			// Tile exists, remove
			PY_DELETE_RELEASE(m_water[i]);
			m_water.erase(m_water.begin() + i);
			--m_numWater;

			return true;
		}
	}

	return false;
}

Bool CLevel::AddEnemy(VECTOR2 _pos, Physics::EType _type)
{
	CheckAgainstGrid(&_pos);

	CEnemy* temp = 0;
	CREATEPOINTER(temp, CEnemy);
	assert(temp);
	switch(_type)
	{
	case Physics::EType::TYPE_BASIC_ENEMY:
		VALIDATE(temp->Initialise("data/art/characters/enemies/basic.png", "data/art/characters/enemies/basic.xml", "data/xml/characters/basicEnemy.xml"));
		break;
	default:
		break;
	}

	_pos.y -= TILE_HEIGHT * 0.5f;
	_pos.y += 1.0f;
	temp->SetPosition(_pos);
	++m_numEnemies;

	m_enemies.push_back(temp);

	return true;
}

Bool CLevel::RemoveEnemy(VECTOR2 _pos)
{
	CheckAgainstGrid(&_pos);
	_pos.y -= TILE_HEIGHT * 0.5f;	
	_pos.y += 1.0f;
	
	// check for existing
	for (Int32 i = 0; i < m_numEnemies; ++i)
	{
		if (m_enemies[i]->GetPosition().x == _pos.x &&
			m_enemies[i]->GetPosition().y == _pos.y)
		{
			// Tile exists, remove
			m_enemies[i]->ShutDown();
			CLEANDELETE(m_enemies[i]);
			m_enemies.erase(m_enemies.begin() + i);
			--m_numEnemies;

			return true;
		}
	}
	return false;
}

Bool CLevel::SetPlayerStart(VECTOR2 _pos)
{
	CheckAgainstGrid(&_pos);
	_pos.y -= TILE_HEIGHT * 0.5f;	
	_pos.y += 1.0f;

	if (0 == m_playable)
	{
		CREATEPOINTER(m_playable, CPlayable);
		assert(m_playable);
		VALIDATE(m_playable->Initialise("data/art/characters/sam/male.png", "data/art/characters/sam/male.xml", "data/xml/characters/sam.xml"));
	}
	m_playable->SetPosition(_pos);

	return true;
}

Bool CLevel::AddMovingPlatform(VECTOR2 _pos)
{
	// conform position to grid
	CheckAgainstGrid(&_pos);

	CPlatform* temp = 0;

	// check for existing platform near the given position
	for (UInt16 i = 0; i < m_platforms.size(); ++i)
	{
		if (m_platforms[i]->CheckPosition(_pos))
		{
			temp = m_platforms[i];
			break;
		}
	}

	// If not found, create new platform
	if (0 == temp)
	{
		CREATEPOINTER(temp, CPlatform);
		assert(temp);	
		temp->Initialise(0, m_tileset, 0, m_platforms.size() + 1);
		temp->CheckPosition(_pos);
		m_platforms.push_back(temp);
		++m_numPlatforms;
	}

	if (temp->AddPosition(_pos, m_tileset))
	{
		//Logger::Write("Moving platform piece added succesfully");
		return true;
	}
	else
	{
		//Logger::Write("Failed to add piece to moving platform");
		return false;
	}
}

Bool CLevel::RemoveMovingPlatform(VECTOR2 _pos)
{
	// conform position to grid
	CheckAgainstGrid(&_pos);

	CPlatform* temp = 0;
	// check for existing platform near the given position
	for (UInt16 i = 0; i < m_platforms.size(); ++i)
	{
		if (m_platforms[i]->CheckPosition(_pos))
		{
			if (m_platforms[i]->RemovePosition(_pos))
			{
				if (m_platforms[i]->GetNumPositions() <= 0)
				{
					m_platforms[i]->ShutDown();
					CLEANDELETE(m_platforms[i]);
					m_platforms.erase(m_platforms.begin() + i);
				}
			}
			return true;
		}
	}
	return false;
}

//
// PRIVATE FUNCTIONS
//

void CLevel::CheckAgainstGrid(VECTOR2* _pos)
{
	// check _pos against the grid first.
	for (Int16 i = 0; i < m_numRects; ++i)
	{		
		if (m_gridRects[i].x <= _pos->x && m_gridRects[i].y <= _pos->y
			&& (m_gridRects[i].x + m_gridRects[i].w) > _pos->x 
			&& (m_gridRects[i].y + m_gridRects[i].h) > _pos->y)
		{
			_pos->x = m_gridRects[i].x + m_gridRects[i].w * 0.5f;
			_pos->y = m_gridRects[i].y + m_gridRects[i].h * 0.5f;
			break;
		}
	}
}

Bool CLevel::CheckForExistingTile(VECTOR2* _pos)
{
	// check for existing
	for (UInt32 i = 0; i < m_water.size(); ++i)
	{
		if (m_water[i]->GetPos().x == _pos->x &&
			m_water[i]->GetPos().y == _pos->y)
		{
			return true;
		}
	}
	for (UInt32 i = 0; i < m_tiles.size(); ++i)
	{
		if (m_tiles[i]->GetPos().x == _pos->x &&
			m_tiles[i]->GetPos().y == _pos->y)
		{
			return true;
		}
	}

	return false;
}