
#pragma once

#ifndef __PAPYRUS_ANIMSPRITE_H__
#define __PAPYRUS_ANIMSPRITE_H__

// Library Includes
#include <SDL_image.h>

// Local Includes
#include "isprite.h"

namespace Papyrus
{
	namespace Sprite
	{
		class CAnimSprite : public ISprite
		{
			// Member Functions
		public:
			CAnimSprite();
			virtual ~CAnimSprite();

			virtual Bool		Initialise(Int8* _spriteSheet, Int8* _setup, Int16 _id);
			virtual Bool		ShutDown();
			
			virtual void		Process(Float32 _delta);
			virtual void		Render();

			virtual void		SetAnim(Int16 _i);
			virtual void		PlayAnim(Int16 _i);

			virtual void		SetAnimSpeed(Float32 _f);

			// Member Variables
		private:
			SDL_Rect*			m_clips;
			Float32				m_timer;
			Float32				m_timePerFrame;
			UInt32				m_numFrames;
			UInt32				m_numRows;
			UInt16				m_currFrame;
			Int16				m_currClip;
			Int16				m_prevAnim;
			Bool				m_playingAnim;
		};
	}
}

#endif // __PAPYRUS_ANIMSPRITE_H__