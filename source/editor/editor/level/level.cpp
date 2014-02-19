
// Library Includes

// Local Includes
#include "level.h"

#include "tile.h"
#include "platform.h"
#include "../../../pirategame/character/enemy.h"

#include "../../parser/parser.h"

using namespace Papyrus;

CLevel::CLevel()
	: m_surface(0)
	, m_grid(0)
	, m_background(0)
	, m_tiles(0)
	, m_levelNumber(INVALID_ID)
	, m_platforms(0)
	, m_numPlatforms(0)
	, m_numEnemies(0)
{

}

CLevel::~CLevel()
{
	
}

Bool CLevel::Initialise(Int8* _setup)
{
	// Make platforms
	Int32 width = Renderer::activeRenderer->GetWidth();

	// Fill grid with rectangles.
	m_numRects = LEVEL_WIDTH/TILE_WIDTH * LEVEL_HEIGHT/TILE_WIDTH;

	m_gridRects = new SDL_Rect[m_numRects];

	m_gridRects[0].x = 0;
	m_gridRects[0].y = static_cast<Int32>(Renderer::activeRenderer->GetHeight() - TILE_HEIGHT);
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
			m_gridRects[i].y -= TILE_HEIGHT;
		}
	}

	VALIDATE(Renderer::activeRenderer->LoadTexture("data/art/editor/grid.png", &m_grid));
	assert(m_grid);

	FileParser::IParser* setup = FileParser::LoadFile(_setup);
	setup->AddRef();
	Int32 tiles = 0;
	VALIDATE(setup->GetValue("tiles", tiles));
	VALIDATE(setup->GetValue("levelNumber", m_levelNumber));
	VALIDATE(setup->GetValue("platforms", m_numPlatforms));
	VALIDATE(setup->GetValue("enemies", m_numEnemies));

	Int8 path[MAX_BUFFER];
	VALIDATE(setup->GetValue("tileset", &m_tileset));
	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/background.png", m_tileset);
	
	m_background = Sprite::CreateSprite(path, 0, false);
	assert(m_background);
	m_background->AddRef();
	PY_WRITETOFILE("Background created");
	
	VECTOR2 pos;
	
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

	Logger::TrackValue(&m_cameraPos, "Camera Position");

	return true;
}

Bool CLevel::ShutDown()
{
	for (Int16 i = 0; i < m_numPlatforms; ++i)
	{
		m_platforms[i]->ShutDown();
		CLEANDELETE(m_platforms[i]);
	}

	// Need to release and shutdown all before deleting because of the tiles knowing of tiles nearby.
	for (UInt16 i = 0; i < m_tiles.size(); ++i)
	{
		m_tiles[i]->Release();
		m_tiles[i]->ShutDown();
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

	for (UInt16 i = 0; i < m_enemies.size(); ++i)
	{
		CLEANDELETE(m_enemies[i]);
	}
	m_enemies.clear();

	PY_SAFE_RELEASE(m_background);

	CLEANARRAY(m_tileset);

	SDL_DestroyTexture(m_grid);

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
	rectPos.y = static_cast<Int32>(Renderer::activeRenderer->GetHeight() - TILE_HEIGHT + m_cameraPos.y);
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

	// Render tiles
	for (UInt16 i = 0; i < m_tiles.size(); ++i)
	{
		m_tiles[i]->Render(m_cameraPos);
	}

	// Render platforms
	for (Int16 i = 0; i < m_numPlatforms; ++i)
	{
		m_platforms[i]->Render();
	}

	// Render Enemies
	for (UInt16 i = 0; i < m_enemies.size(); ++i) 
	{
		m_enemies[i]->Render(m_cameraPos);
	}
}

Bool CLevel::Save()
{
	FileParser::IParser* save = FileParser::CreateParser("1.json");
	assert(save);
	save->AddRef();
	
	VALIDATE(save->AddValue("playerStart", VECTOR2(225.0f, 831.0f)));
	VALIDATE(save->AddValue("tileset", m_tileset));
	VALIDATE(save->AddValue("tiles", m_numTiles));
	VALIDATE(save->AddValue("platforms", m_numPlatforms));
	VALIDATE(save->AddValue("enemies", m_numEnemies));
	VALIDATE(save->AddValue("levelNumber", 1));

	Int8 text[MAX_BUFFER];
	for (UInt16 i = 0; i < m_numTiles; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "%i-pos", i + 1);
		VALIDATE(save->AddValue(text, m_tiles[i]->GetPos()));
		SDL_snprintf(text, MAX_BUFFER, "%i-type", i + 1);
		VALIDATE(save->AddValue(text, m_tiles[i]->GetType()));
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
	temp->SetPosition(_pos);
	++m_numEnemies;

	m_enemies.push_back(temp);

	return true;
}

Bool CLevel::RemoveEnemy(VECTOR2 _pos)
{
	CheckAgainstGrid(&_pos);
	_pos.y -= TILE_HEIGHT * 0.5f;	
	
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