#pragma once

#ifndef __PAPYRUS_UI_H__
#define __PAPYRUS_UI_H__

// Library Includes

// Local Includes
#include "interface.h"

namespace Papyrus
{
	namespace UI
	{
		/*
		* Initialise the user interface system.
		*
		* @return	Returns true when initialised successfully, false otherwise.
		*/
		Bool		Initialise();

		/*
		* Shuts the user interface system down and cleans memory.
		*
		* @return	Returns true on success, false otherwise.
		*/
		Bool		ShutDown();

		/*
		* Renders the interface to screen.
		*
		* @return	Returns void.
		*/
		void		Render();

		/*
		* Creates an interface of the given type and filepath.
		*
		* @param	_path		Path to save the interface on.
		* @return	Returns a pointer to an IUIInterface object if successfull, 0 otherwise.
		*/
		IUIInterface* CreateInterface(Int8* _path, Bool _interactive);

		/*
		* Loads a file into a parser.
		*
		* @param	_filePath		Interface to load.
		* @return	Returns a pointer to an IUIInterface object if the file was loaded successfully. Otherwise returns 0.
		*/
		IUIInterface* LoadInterface(Int8* _filePath);

		/*
		* Flushes a parser from memory.
		*
		* @param	Interface to flush.
		* @return	Returns 0 if the interface was flushed successfully. Returns the pointer if not.
		*/
		IUIInterface* FlushInterface(IUIInterface* _interface);

		/*
		* None of the below variables should be touched outside of program intialisation.
		*/

		// Holds pointers to the loaded interfaces
		extern IUIInterface**	interfaces;

		extern UInt16			numInterfaces;
	}
}

#endif // __PAPYRUS_UI_H__