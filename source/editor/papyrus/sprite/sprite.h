
#pragma once

#ifndef __PAPYRUS_SPRITE_H__
#define __PAPYRUS_SPRITE_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "isprite.h"
#include "../renderer/renderer.h"
#include "../renderer/irenderer.h"
#include "../logging/logger.h"

namespace Papyrus
{
	namespace Sprite
	{
		/*
		* Initialises the sprite system. Must be called before any sprites can be created.
		*
		* @return	Returns true on successfull initialisation, false otherwise.
		*/
		Bool		Initialise();
		
		/*
		* Shuts the sprite system down and cleans up memory.
		*
		* @return	Returns true on successfull shutdown, false otherwise.
		*/
		Bool		ShutDown();
		
		/*
		* Creates a sprite.
		*
		* @param	_spriteSheet		Path to the PNG image to use. 
		* @param	_animated			To make an animated sprite or not.
		* @return	Returns a pointer to the created sprite, 0 if creation failed.
		*/
		ISprite*	CreateSprite(Int8* _spriteSheet, Int8* _setup, Bool _animated);
		
		/*
		* Removes a sprite from the system.
		*
		* @param	_sprite				The sprite to remove.
		* @param	_delete				To completely delete the sprite from memory, or just remove it from this system.
		* @return	Returns 0 when successfully deleted, otherwise returns the _sprite pointer.
		*/
		ISprite*	FlushFile(ISprite* _sprite, Bool _delete = true);

		Int32		SetSpritePosition(lua_State* L);
		Int32		SetSpriteScale(lua_State* L);

		/*
		* None of the below variables should be touched outside of program intialisation.
		*/

		// Holds pointer for all active ISprite objects.
		extern ISprite**		activeSprites;

		// Holds pointer to active renderer for loading textures.
		extern Renderer::IRenderer* activeRenderer;
		
		// Maximum number of sprites that will exist at one time.
		// Needs to be set before Initialise is run as this determines the size of the above arrays.
		extern UInt16			maxNumSprites;
	}
}

#endif // __PAPYRUS_SPRITE_H__