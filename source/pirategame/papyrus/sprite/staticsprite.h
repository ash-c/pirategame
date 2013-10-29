
#pragma once

#ifndef __PAPYRUS_STATICSPRITE_H__
#define __PAPYRUS_STATICSPRITE_H__

// Library Includes
#include <SDL_image.h>

// Local Includes
#include "isprite.h"

namespace Papyrus
{
	namespace Sprite
	{
		class CStaticSprite : public ISprite
		{
			// Member Functions
		public:
			CStaticSprite();
			virtual ~CStaticSprite();

			virtual Bool		Initialise(Int8* _spriteSheet);
			virtual Bool		ShutDown();
			
			virtual void		Render();

			// Member Variables
		private:
			SDL_Texture*		m_SpriteSheet;
			SDL_Rect			m_Sprite;
		};
	}
}

#endif // __PAPYRUS_STATICSPRITE_H__