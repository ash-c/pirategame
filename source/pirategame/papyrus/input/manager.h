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
			// Default constructor
			CInputManager();
			// Default destructor
			~CInputManager();

			/*
			* Initialise the input manager.
			*
			* @return	Returns true on success, false otherwise.
			*/
			Bool				Initialise();
			
			/*
			* Shuts the input manager down and cleans memory.
			*
			* @return	Returns true on success, false otherwise.
			*/
			Bool				ShutDown();
			
			/*
			* Processes the input and notifies any observers.
			*
			* @param	_delta		Time passed since last game frame.
			* @return	Returns void.
			*/
			void				Process(Float32 _delta);
			
			/*
			* Allows an observer to register itself with the manager.
			*
			* @param	_obs		Pointer to the observer trying to register.
			* @return	Returns true if registered successfully, false otherwise.
			*/
			Bool				Register(IInputObserver* _obs);

			// Member Variables
		protected:
			static const Int16	MAX_OBSERVERS = 10;

			IInputObserver**	m_observers;
			SDL_Joystick*		m_joystick;
		};
	}
}

#endif // __PAPYRUS_INPUT_MANAGER_H__
