#pragma once

#ifndef __PAPYRUS_INPUT_MANAGER_H__
#define __PAPYRUS_INPUT_MANAGER_H__

// Library Includes
#include <SDL.h>

// Local Includes
#include "../defines.h"
#include "observer.h"

namespace Papyrus
{
	namespace Input
	{
		class CInputManager
		{
			// Member Functions
		public:
			CInputManager();
			~CInputManager();

			Bool				Initialise();
			void				Process(Float32 _delta);

			Bool				Register(IInputObserver* _obs);

			// Member Variables
		protected:
			const Int16			MAX_OBSERVERS = 10;

			IInputObserver**	m_observers;
		};
	}
}

#endif // __PAPYRUS_INPUT_MANAGER_H__
