#pragma once

#ifndef __PAPYRUS_UI_OBJECT_H__
#define __PAPYRUS_UI_OBJECT_H__

// Library Includes

// Local Includes
#include "../core/utility/refcount.h"
#include "../sprite/sprite.h"

namespace Papyrus
{
	namespace UI
	{
		class CUIObject : public TRefCount<CUIObject>
		{
			// Member Functions
		public:
			// Default Constructor
			CUIObject()
				: m_sprite(0)
				, m_active(true)
			{
			}

			// Default Destructor
			virtual ~CUIObject()
			{
			}

			/*
			* Initialise this UI object.
			*
			* @return	Returns true when initialised successfully, false otherwise.
			*/
			virtual Bool	Initialise(Int8* _luaFile, Int8* _luaFunc, Int8* _sprite, VECTOR2 _pos, UInt16 _w, UInt16 _h) = 0;

			/*
			* Shuts this UI object down and cleans memory.
			*
			* @return	Returns true when initialised successfully, false otherwise.
			*/
			virtual Bool	ShutDown()
			{
				PY_SAFE_RELEASE(m_sprite);
				return true;
			}

			/*
			* Processes the UI object.
			*
			* @param	_delta			Time passed since last process.
			* @return	Returns void.
			*/
			virtual void	Process(Float32 _delta) = 0;

			/*
			* Renders this object to screen.
			*
			* @return	Returns void.
			*/
			virtual void	Render() = 0;

			/*
			* Checks if this UI object is active.
			*
			* @return	Returns the active status of the UI object.
			*/
			virtual Bool	IsActive() { return m_active; }

			/*
			* Sets the active status of the UI object.
			*
			* @param	_b			The new active status of the UI object.
			* @return	Returns void.
			*/
			virtual void	SetActive(Bool _b) 
			{ 
				m_active = _b; 
			}

			/*
			* Set the position of the UI object.
			*
			* @param	_pos		The position to set.
			* @return	Returns void.
			*/
			virtual void	SetPosition(VECTOR2 _pos) = 0;

			virtual Int32	GetHeight() = 0;
			virtual Int32	GetWidth() = 0;

			// Member Variables
		protected:
			Sprite::ISprite*	m_sprite;
			Bool				m_active;
		};
	}
}

#endif // __PAPYRUS_UI_OBJECT_H__