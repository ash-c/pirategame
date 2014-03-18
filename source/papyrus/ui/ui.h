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
		* Creates an interface of the given type and filepath.
		*
		* @param	_path		Path to save the interface on.
		* @return	Returns a pointer to an IUIInterface object if successfull, 0 otherwise.
		*/
		IUIInterface* CreateInterface(Int8* _path, Bool _interactive = true, Bool _context = false);

		/*
		* Loads a file into a parser.
		*
		* @param	_filePath		Interface to load.
		* @param	_interactive	Create an interactive or static interface.
		* @param	_context		Create a context interface.
		* @return	Returns a pointer to an IUIInterface object if the file was loaded successfully. Otherwise returns 0.
		*/
		IUIInterface* LoadInterface(Int8* _filePath, Bool _interactive = true, Bool _context = false);

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

		extern Float32			hScale;
		extern Float32			wScale;
		extern UInt16			numInterfaces;
	}
}

#endif // __PAPYRUS_UI_H__