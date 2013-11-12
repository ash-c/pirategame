
// Library Includes
#include <SDL_image.h>

// Local Includes
#include "sprite.h"
#include "isprite.h"
#include "staticsprite.h"

using namespace Papyrus;

Sprite::ISprite**		Sprite::activeSprites = 0;
Renderer::IRenderer*	Sprite::activeRenderer = 0;
UInt16					Sprite::maxNumSprites = 50;

Bool Sprite::Initialise()
{
	assert(maxNumSprites > 0);

	if ((IMG_Init(IMG_INIT_PNG) & (IMG_INIT_PNG)) != (IMG_INIT_PNG))
	{
		return false;
	}

	activeSprites = new ISprite*[maxNumSprites];
	SDL_memset(activeSprites, 0, sizeof(ISprite*) * maxNumSprites);

	activeRenderer = Renderer::activeRenderer;
	activeRenderer->AddRef();

	return true;
}

Bool Sprite::ShutDown()
{
	PY_CLEANARRAY(activeSprites, maxNumSprites);

	PY_SAFE_RELEASE(activeRenderer);
	IMG_Quit();
	return true;
}

 Sprite::ISprite* Sprite::CreateSprite(Int8* _spriteSheet, Bool _animated)
{
	assert(_spriteSheet != 0 && "Need a spritesheet path");

	Sprite::ISprite* sprite = 0;
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