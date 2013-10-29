
// Library Includes

// Local Includes
#include "staticsprite.h"
#include "../renderer/renderer.h"

using namespace Papyrus;

Sprite::CStaticSprite::CStaticSprite()
	: m_SpriteSheet(0)
{
	m_Sprite.x = 0;
	m_Sprite.y = 0;
	m_Sprite.w = 0;
	m_Sprite.h = 0;
}

Sprite::CStaticSprite::~CStaticSprite()
{
}

Bool Sprite::CStaticSprite::Initialise(Int8* _spriteSheet)
{
	Renderer::activeRenderer->LoadTexture(_spriteSheet, &m_SpriteSheet);

	if (nullptr == m_SpriteSheet) 
	{
			return false;
	}

	SDL_QueryTexture(m_SpriteSheet, NULL, NULL, &m_Sprite.w, &m_Sprite.h);

	return true;
}

Bool Sprite::CStaticSprite::ShutDown()
{
	SDL_DestroyTexture(m_SpriteSheet);
	return true;
}

void Sprite::CStaticSprite::Render()
{
	Renderer::activeRenderer->Render(m_SpriteSheet, &m_Sprite, NULL);
}