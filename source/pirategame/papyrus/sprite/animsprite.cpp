
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
	Int32 w, h;
	VALIDATE(setup->GetValue("rows", m_numRows));
	VALIDATE(setup->GetValue("frames", m_numFrames));
	VALIDATE(setup->GetValue("width", w));
	VALIDATE(setup->GetValue("height", h));
	VALIDATE(setup->GetValue("timer", m_timePerFrame));
	FileParser::FlushFile(setup);
	m_sprite.w = w;
	m_sprite.h = h;

	m_clips = new SDL_Rect[m_numRows];

	for (UInt16 i = 0; i < m_numRows; ++i)
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

void Sprite::CAnimSprite::Process(Float32 _delta)
{
	m_timer += _delta;

	if (m_timer > m_timePerFrame)
	{
		++m_currFrame;

		if (m_currFrame == m_numFrames)
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
			m_clips[m_currClip].x += m_sprite.w;
		}
		m_timer -= m_timePerFrame;
	}
}

void Sprite::CAnimSprite::Render()
{
	Renderer::activeRenderer->Render(m_spriteSheet, &m_sprite, &m_clips[m_currClip]);
}

void Sprite::CAnimSprite::SetAnim(Int16 _i)
{
	if (m_currClip != _i)
	{
		m_clips[m_currClip].x = 0;
		m_currFrame = 0;
		m_currClip = _i;
		m_timer -= m_timePerFrame;
	}
}

void Sprite::CAnimSprite::PlayAnim(Int16 _i)
{
	if (m_currClip != _i)
	{
		m_clips[m_currClip].x = 0;
		m_prevAnim = m_currClip;
		m_currFrame = 0;
		m_currClip = _i;
		m_timer -= m_timePerFrame;
	}
}