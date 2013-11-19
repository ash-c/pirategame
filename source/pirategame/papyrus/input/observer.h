#pragma once

#ifndef __PAPYRUS_INPUT_OBSERVER_H__
#define __PAPYRUS_INPUT_OBSERVER_H__

// Library Includes
#include <SDL.h>

// Local Includes
#include "../defines.h"

namespace Papyrus
{
	namespace Input
	{
		class IInputObserver
		{
			// Member Functions
		public:
			IInputObserver() {}
			virtual ~IInputObserver() {}

			/*
			* Function used by the input manager to tell an object that an input event has happened.
			*
			* @param	_e		Pointer to an SDL_Event object.
			* @return	Returns void.
			*/
			virtual void		Notify(SDL_Event* _e) = 0;

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_INPUT_OBSERVER_H__