
// Library Includes

// Local Includes
#include "animsprite.h"
#include "../renderer/renderer.h"
#include "../parser/parser.h"

using namespace Papyrus;

Sprite::CAnimSprite::CAnimSprite()
	: m_clips(0)
	, m_timer(0.0f)
	, m_currFrame(0)
	, m_currClip(0)
	, m_prevAnim(INVALID_ID)
{
}

Sprite::CAnimSprite::~CAnimSprite()
{
	CLEANARRAY(m_clips);
}

Bool Sprite::CAnimSprite::Initialise(Int8* _spriteSheet, Int8* _setup, Int16 _id)
{
	Renderer::activeRenderer->LoadTexture(_spriteSheet, &m_spriteSheet);

	if (nullptr == m_spriteSheet) 
	{
			return false;
	}
	
	// Set up clipping for the spritesheet
	FileParser::IParser* setup = FileParser::LoadFile(_setup);
	setup->AddRef();
	Int32 rows, frames, w, h;
	VALIDATE(setup->GetValue("rows", rows));
	VALIDATE(setup->GetValue("frames", frames));
	VALIDATE(setup->GetValue("width", w));
	VALIDATE(setup->GetValue("height", h));
	FileParser::FlushFile(setup);
	m_sprite.w = w;
	m_sprite.h = h;

	Int32 size = rows * frames;
	m_clips = new SDL_Rect[rows];

	for (Int16 i = 0; i < rows; ++i)
	{
		m_clips[i].x = i * w;
		m_clips[i].y = i * h;
		m_clips[i].w = w;
		m_clips[i].h = h;
	}

	ISprite::Initialise(_spriteSheet, _setup, _id);

	return true;
}

Bool Sprite::CAnimSprite::ShutDown()
{
	SDL_DestroyTexture(m_spriteSheet);
	return true;
}

void Sprite::CAnimSprite::Process(Float32 _fDelta)
{
	m_timer += _fDelta;

	if (m_timer > 1.0f)
	{
		++m_currFrame;

		if (m_currFrame == 6)
		{
			m_clips[m_currClip].x = 0;
			m_currFrame = 0;
			if (INVALID_ID != m_prevAnim)
			{
				m_currClip = m_prevAnim;
				m_prevAnim = INVALID_ID;
			}
		}
		else
		{
			m_clips[m_currClip].x += 50;
		}
		m_timer = 0.0f;
	}
}

void Sprite::CAnimSprite::Render()
{
	Renderer::activeRenderer->Render(m_spriteSheet, &m_sprite, &m_clips[m_currClip]);
}

void Sprite::CAnimSprite::SetAnim(Int16 _i)
{
	m_clips[m_currClip].x = 0;
	m_currFrame = 0;
	m_currClip = _i;
}

void Sprite::CAnimSprite::PlayAnim(Int16 _i)
{
	m_clips[m_currClip].x = 0;
	m_prevAnim = m_currClip;
	m_currFrame = 0;
	m_currClip = _i;
}