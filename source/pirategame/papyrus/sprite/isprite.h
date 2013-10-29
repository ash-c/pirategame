
#pragma once

#ifndef __PAPYRUS_ISPRITE_H__
#define __PAPYRUS_ISPRITE_H__

// Library Includes
#include <SDL.h>

// Local Includes
#include "../core/utility/refcount.h"

namespace Papyrus
{
	namespace Sprite
	{
		class ISprite : public TRefCount<ISprite>
		{
			// Member Functions
		public:
			// Default Constructor
			ISprite()
				: m_Texture(0)
			{
			}

			// Default Destructor
			virtual ~ISprite()
			{
			}

			virtual Bool		Initialise(Int8* _spriteSheet) = 0;
			virtual Bool		ShutDown() = 0;

			// Member Variables
		protected:
			SDL_Texture*		m_Texture;
		};
	}
}

#endif // __PAPYRUS_ISPRITE_H__