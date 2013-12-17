
// Library Includes

// Local Includes
#include "platform.h"
#include "tile.h"

CPlatform::CPlatform()
	: m_tiles(0)
	, m_numTiles(0)
{
}

CPlatform::~CPlatform()
{
}

Bool CPlatform::Initialise(FileParser::IParser* _setup, Int32 _number, Int32 _platNum)
{
	m_numTiles = _number;
	_setup->AddRef();

	m_tiles = new CTile*[m_numTiles];
	assert(m_tiles);
	SDL_memset(m_tiles, 0, sizeof(CTile*) * m_numTiles);

	Int8 text[MAX_BUFFER];
	VECTOR2 pos;
	UInt32 type = 0;

	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		CREATEPOINTER(m_tiles[i], CTile);
		SDL_snprintf(text, MAX_BUFFER, "%iplatform-%ipos", _platNum, i + 1);
		VALIDATE(_setup->GetValue(text, pos));
		SDL_snprintf(text, MAX_BUFFER, "%iplatform-%itype", _platNum, i + 1);
		VALIDATE(_setup->GetValue(text, type));
		VALIDATE(m_tiles[i]->Initialise("data/art/levels/tiles.png", pos, static_cast<ETileType>(type)));
		m_tiles[i]->AddRef();
	}

	_setup->Release();
	return true;
}

Bool CPlatform::ShutDown()
{
	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		PY_DELETE_RELEASE(m_tiles[i]);
	}

	return true;
}

void CPlatform::Render()
{
	for (Int16 i = 0; i < m_numTiles; ++i)
	{
		m_tiles[i]->Render();
	}
}