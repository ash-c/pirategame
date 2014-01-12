
// Library Includes

// Local Includes
#include "staticsprite.h"
#include "../renderer/renderer.h"

using namespace Papyrus;

Sprite::CStaticSprite::CStaticSprite()
{
	m_clip.x = 0;
	m_clip.y = 0;
}

Sprite::CStaticSprite::~CStaticSprite()
{
}

Bool Sprite::CStaticSprite::Initialise(Int8* _spriteSheet, Int8* _setup, Int16 _id)
{
	Renderer::activeRenderer->LoadTexture(_spriteSheet, &m_spriteSheet);

	if (nullptr == m_spriteSheet) 
	{
			return false;
	}

	SDL_QueryTexture(m_spriteSheet, NULL, NULL, &m_sprite.w, &m_sprite.h);
	m_clip.w = m_sprite.w;
	m_clip.h = m_sprite.h;

	ISprite::Initialise(_spriteSheet, _setup, _id);	

	return true;
}

Bool Sprite::CStaticSprite::ShutDown()
{
	SDL_DestroyTexture(m_spriteSheet);
	return true;
}

void Sprite::CStaticSprite::Render()
{
	Renderer::activeRenderer->Render(m_spriteSheet, &m_sprite, &m_clip);
}

void Sprite::CStaticSprite::SetClip(SDL_Rect* _clip)
{
	m_clip.x = _clip->x;
	m_clip.y = _clip->y;
	m_clip.w = _clip->w;
	m_clip.h = _clip->h;
}