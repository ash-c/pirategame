
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

		/*
		* None of the below variables should be touched outside of program intialisation.
		*/

		extern ISprite**		activeSprites;

		extern Renderer::IRenderer* activeRenderer;

		extern UInt16			maxNumSprites;
	}
}

#endif // __PAPYRUS_SPRITE_H__