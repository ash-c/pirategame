
// Library Includes

// Local Includes
#include "staticsprite.h"
#include "../renderer/renderer.h"

using namespace Papyrus;

Sprite::CStaticSprite::CStaticSprite()
{
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

	ISprite::Initialise(_spriteSheet, _setup, _id);

	return true;
}

Bool Sprite::CStaticSprite::ShutDown()
{
	SDL_DestroyTexture(m_spriteSheet);
	return true;
}

void Sprite::CStaticSprite::Process(Float32 _fDelta)
{
}

void Sprite::CStaticSprite::Render()
{
	Renderer::activeRenderer->Render(m_spriteSheet, &m_sprite, NULL);
}