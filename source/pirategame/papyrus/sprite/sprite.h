
#pragma once

#ifndef __PAPYRUS_SPRITE_H__
#define __PAPYRUS_SPRITE_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "isprite.h"
#include "../renderer/renderer.h"
#include "../renderer/irenderer.h"

namespace Papyrus
{
	namespace Sprite
	{
		Bool		Initialise();
		Bool		ShutDown();

		ISprite*	CreateSprite(Int8* _spriteSheet, Bool _animated);

		extern Renderer::IRenderer* activeRenderer;
	}
}

#endif // __PAPYRUS_SPRITE_H__