#pragma once

#ifndef __PAPYRUS_INPUT_H__
#define __PAPYRUS_INPUT_H__

// Library Includes

// Local Includes
#include "manager.h"
#include "observer.h"

namespace Papyrus
{
	namespace Input
	{
		/*
		* Initialises the input system.
		*
		* @return	Returns true when initialised successfully, false otherwise.
		*/
		Bool		Initialise();

		/*
		* Shuts the input system down and cleans memory.
		*
		* @return	Returns true on success, false otherwise.
		*/
		Bool		ShutDown();

		// Use this variable to register observers
		extern CInputManager*	inputManager;
	}
}

#endif // __PAPYRUS_INPUT_H__