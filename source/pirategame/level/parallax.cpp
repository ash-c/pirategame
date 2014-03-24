
// Library Includes

// Local Includes
#include "parallax.h"

CParallax::CParallax()
	: m_animated(false)
{
	m_sprites.clear();
	m_positions.clear();
}

CParallax::~CParallax()
{
}

Bool CParallax::Initialise(FileParser::IParser* _setup, Int8* _num, Bool _animate)
{
	assert(0 != _setup);
	m_animated = _animate;

	Int32 objects = 0;
	VALIDATE(_setup->GetValue("objects", objects, _num));

	return true;
}

Bool CParallax::ShutDown()
{
	for (UInt16 i = 0; i < m_sprites.size(); ++i)
	{
		Sprite::FlushFile(m_sprites[i]);
		m_sprites[i] = 0;
	}
	m_sprites.clear();
	return true;
}

void CParallax::Process(Float32 _delta)
{
	for (UInt16 i = 0; i < m_sprites.size(); ++i)
	{
		m_sprites[i]->Process(_delta);
	}
}

void CParallax::Render(VECTOR2 _cameraPos)
{
	for (UInt16 i = 0; i < m_sprites.size(); ++i)
	{
		m_sprites[i]->SetPosition(static_cast<Int32>(m_positions[i].x + _cameraPos.x), static_cast<Int32>(m_positions[i].y - _cameraPos.y));
		m_sprites[i]->Render();
	}
}