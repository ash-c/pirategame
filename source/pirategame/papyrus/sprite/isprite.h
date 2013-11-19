
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
				: m_spriteSheet(0)
				, m_id(INVALID_ID)
				, m_filePath(0)
			{
				m_sprite.x = 0;
				m_sprite.y = 0;
				m_sprite.w = 0;
				m_sprite.h = 0;
			}

			// Default Destructor
			virtual ~ISprite()
			{
				CLEANARRAY(m_filePath);
			}

			/*
			* Initialises this sprite.
			*
			* @param	_spriteSheet	Path to the spritesheet to use.
			* @return	Returns true if successfull, false otherwise.
			*/
			virtual Bool		Initialise(Int8* _spriteSheet, Int8* _setup, Int16 _id)
			{
				m_id = _id;
				Int16 length = SDL_strlen(_spriteSheet) + 1;
				m_filePath = new Int8[length];
				SDL_strlcpy(m_filePath, _spriteSheet, length);
				return true;
			}

			/*
			* Shuts this sprite down and cleans up memory.
			*
			* @return	Returns true if successfull, false otherwise.
			*/
			virtual Bool		ShutDown() = 0;

			/*
			* @return	Returns the ID of the sprite.
			*/
			virtual Int16		GetID()
			{
				return m_id;
			}

			/*
			* Processes the sprite.
			*
			* @param	_fDelta			Time passed since last frame.
			* @return	Returns void.
			*/
			virtual void		Process(Float32 _fDelta) = 0;

			/*
			* Renders this sprite to screen.
			*
			* @return	Returns void.
			*/
			virtual void		Render() = 0;

			/*
			* Sets the position of the sprite.
			*
			* @param	_x				X position to set.
			* @param	_y				Y position to set.
			* @return	Returns void.
			*/
			virtual void		SetPosition(Int32 _x, Int32 _y)
			{
				m_sprite.x = static_cast<Int32>(_x - m_sprite.w * 0.5f);
				m_sprite.y = static_cast<Int32>(_y - m_sprite.h * 0.5f);
			}

			/*
			* Sets the width and height of the sprite.
			*
			* @param	_w				The new width of the sprite.
			* @param	_h				The new height of the sprite.
			* @return	Returns void.
			*/
			virtual void		SetScale(Int32 _w, Int32 _h)
			{
				m_sprite.w = _w;
				m_sprite.h = _h;
			}

			/*
			* Set which animation to play.
			*
			* @param	_i				Which animation to start.
			* @return	Returns void.
			*/
			virtual void		SetAnim(Int16 _i) {}

			/*
			* Play an animation once.
			*
			* @param	_i				Which animation to play.
			* @return	Returns void.
			*/
			virtual void		PlayAnim(Int16 _i) {}
			
			/*
			* Compares the given file path against the one loaded into this sprite.
			*
			* @param	_path			Path to compare.
			* @return	Returns true if _path matches this sprite ie:this spritesheet has already been loaded, false otherwise.
			*/
			virtual Bool		CompareFilePath(Int8* _path)
			{
				return !SDL_strcmp(_path, m_filePath);
			}

			// Member Variables
		protected:
			SDL_Texture*		m_spriteSheet;
			SDL_Rect			m_sprite;
			Int16				m_id;
			Int8*				m_filePath;
		};
	}
}

#endif // __PAPYRUS_ISPRITE_H__