
#pragma once

#ifndef __PAPYRUS_ISPRITE_H__
#define __PAPYRUS_ISPRITE_H__

// Library Includes
#include <SDL.h>

// Local Includes
#include "../core/utility/refcount.h"
#include "../parser/parser.h"

class CPlayable;

namespace Papyrus
{
	namespace Sprite
	{
		class FileParser::IParser;

		class ISprite : public TRefCount<ISprite>
		{
			// Member Functions
		public:
			// Default Constructor
			ISprite()
				: m_spriteSheet(0)
				, m_setup(0)
				, m_id(INVALID_ID)
			{
				m_sprite.x = 0;
				m_sprite.y = 0;
				m_sprite.w = 0;
				m_sprite.h = 0;
			}

			// Default Destructor
			virtual ~ISprite()
			{
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
			* Sets the centre position of the sprite.
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

			virtual VECTOR2		GetPosition()
			{
				return VECTOR2(static_cast<Float32>(m_sprite.x), static_cast<Float32>(m_sprite.y));
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
			* Returns the scale of the sprite.
			*
			* @return	Returns a vector2, where the x is the width, and the y is the height of the sprite.
			*/
			virtual VECTOR2		GetScale()
			{
				return VECTOR2(static_cast<Float32>(m_sprite.w), static_cast<Float32>(m_sprite.h));
			}

			/*
			* Sets the area of a sprite to render for a static sprite. Not implemented for animated sprites.
			*
			* @param	_clip			The area to render.
			* @return	Returns void.
			*/
			virtual void		SetClip(SDL_Rect* _clip) { }

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
			* @param	_override		Overrides currently playing animation if there is one.
			* @return	Returns void.
			*/
			virtual void		PlayAnim(Int16 _i, Bool _override = false) {}

			/*
			* Gets which animation is currently being played.
			*
			* @return	Returns which animation is being played.
			*/
			virtual Int16		GetAnim() { return INVALID_ID; }

			/*
			* Set the animation speed, only implemented for animated sprites.
			*
			* @param	_f				New animation speed.
			* @return	Returns void.
			*/
			virtual void		SetAnimSpeed(Float32 _f) {}

			// Member Variables
		protected:
			SDL_Texture*		m_spriteSheet;
			SDL_Rect			m_sprite;
			FileParser::IParser*	m_setup;
			Int16				m_id;
		};
	}
}

#endif // __PAPYRUS_ISPRITE_H__