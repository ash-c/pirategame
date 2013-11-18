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

			virtual void		Notify(SDL_Event _e) = 0;

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_INPUT_OBSERVER_H__