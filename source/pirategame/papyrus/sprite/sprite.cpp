
// Library Includes
#include <SDL_image.h>

// Local Includes
#include "sprite.h"
#include "isprite.h"
#include "staticsprite.h"

using namespace Papyrus;

Renderer::IRenderer* Sprite::activeRenderer = 0;

Bool Sprite::Initialise()
{
	if ((IMG_Init(IMG_INIT_PNG) & (IMG_INIT_PNG)) != (IMG_INIT_PNG))
	{
		return false;
	}

	activeRenderer = Renderer::activeRenderer;
	activeRenderer->AddRef();

	return true;
}

Bool Sprite::ShutDown()
{
	activeRenderer->Release();
	IMG_Quit();
	return true;
}

Sprite::ISprite* Sprite::CreateSprite(Int8* _spriteSheet, Bool _animated)
{
	assert(_spriteSheet != 0 && "Need a spritesheet path");

	ISprite* sprite = 0;
	if (_animated)
	{
	}
	else
	{
		CREATEPOINTER(sprite, CStaticSprite);
		assert(sprite);
	}

	VALIDATE(sprite->Initialise(_spriteSheet));

	return sprite;
}