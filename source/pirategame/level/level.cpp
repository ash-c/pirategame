
// Library Includes

// Local Includes
#include "level.h"

#include "..\character\character.h"
#include "..\character\playable.h"
#include "tile.h"
#include "platform.h"

CLevel::CLevel()
	: m_background(0)
	, m_playable(0)
	, m_tiles(0)
	, m_levelNumber(INVALID_ID)
	, m_platforms(0)
	, m_static(0)
	, m_numPlatforms(0)
	, m_screenW(0)
{

}

CLevel::~CLevel()
{
	
}

Bool CLevel::Initialise(Int8* _setup)
{
	// Make platforms
	m_screenW = Renderer::activeRenderer->GetWidth();
	m_screenH = Renderer::activeRenderer->GetHeight();

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

	if (0 < m_numTiles)
	{
		CREATEPOINTER(m_playable, CPlayable);
		assert(m_playable);
		m_playable->Initialise("data/art/characters/sam/male.png", "data/art/characters/sam/male.xml", "data/xml/characters/sam.xml");
		PY_WRITETOFILE("Player created");

		VALIDATE(setup->GetValue("playerStart", pos));
		m_playable->SetPosition(pos);
	}

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
	
	/*Renderer::activeRenderer->CreateTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, LEVEL_WIDTH, LEVEL_HEIGHT, &m_static);
	assert(m_static);

	VALIDATE(Renderer::activeRenderer->SetRenderTarget(m_static));

	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		m_tiles[i]->Render(m_cameraPos);
	}

	VALIDATE(Renderer::activeRenderer->SetRenderTarget(NULL));*/
	
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
	SDL_DestroyTexture(m_static);

	CLEANARRAY(m_tiles);

	PY_SAFE_RELEASE(m_background);

	if (0 != m_playable)
	{
		m_playable->ShutDown();
		CLEANDELETE(m_playable);
	}

	Logger::TrackValue(&m_cameraPos, "Camera Position");

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
		Int32 scrollUp = static_cast<Int32>(m_screenH * 0.5f);

		if (pos.x >= scrollLeft && pos.x + scrollLeft < LEVEL_WIDTH)
		{
			m_cameraPos.x = -(pos.x - scrollLeft); 
		}
		if (pos.y < scrollUp)
		{
			m_cameraPos.y = -(pos.y - scrollUp);
			if (m_cameraPos.y > LEVEL_HEIGHT - Renderer::activeRenderer->GetHeight())
			{
				m_cameraPos.y = static_cast<Float32>(LEVEL_HEIGHT - Renderer::activeRenderer->GetHeight());
			}
		}
		else
		{
			m_cameraPos.y = 0.0f;
		}

		Physics::camPosition = m_cameraPos;
	}

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
		m_tiles[i]->Render(m_cameraPos);
	}

	/*SDL_Rect dst;
	dst.w = LEVEL_WIDTH;
	dst.h = LEVEL_HEIGHT;
	dst.x = m_cameraPos.x;
	dst.y = m_cameraPos.y;
	Renderer::activeRenderer->Render(m_static, &dst, NULL);

	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		m_tiles[i]->Render(m_cameraPos);
	}*/

	for (Int16 i = 0; i < m_numPlatforms; ++i)
	{
		m_platforms[i]->Render();
	}

	if (0 != m_playable)
	{
		m_playable->Render(m_cameraPos);
	}
}