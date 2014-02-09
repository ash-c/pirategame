//
// parser.h
//
// This file defines functions related to handling text files of various formats.
//
#pragma once

#ifndef __PAPYRUS_FILEPARSER_H__
#define __PAPYRUS_FILEPARSER_H__

// Library Includes

// Local Includes
#include "../../defines.h"
#include "iparser.h"

namespace Papyrus
{
	namespace FileParser
	{
		/*
		* Initialises the FileParser. Must be called before being used.
		*
		* @return	Returns true when initialised successfully.
		*/
		Bool Initialise();

		/*
		* Processes parsers, deleting ones that have existed for longer than the time delay.
		*
		* @param	_fDelta		Time passed since last process.
		* @return	Returns void.
		*/
		void Process(Float32 _fDelta);

		/*
		* Ensures the FileParser has shut down correctly.
		*
		* @return	Returns true when shut down successfully.
		*/
		Bool ShutDown();

		/*
		* Creates a parser of the given type and filepath.
		*
		* @param	_path		Path to save the parser on.
		* @return	Returns a pointer to an IParser object if successfull, 0 otherwise.
		*/
		IParser* CreateParser(Int8* _path);

		/*
		* Loads a file into a parser.
		*
		* @param	_filePath		File to load.
		* @return	Returns a pointer to an IParser object if the file was loaded successfully. Otherwise returns 0.
		*/
		IParser* LoadFile(Int8* _filePath);

		/*
		* Flushes a parser from memory.
		*
		* @param	_filePath		File to flush.
		* @param	_delete			If true, function calls Release and ShutDown on _parser before deleting it.
		*							Otherwise the calling class needs to do this.
									Only really used in the IParser destructor, to allow for manual deletion of pointers.
		* @return	Returns 0 if the file was flushed successfully. Returns the pointer if not.
		*/
		IParser* FlushFile(IParser* _parser, Bool _delete = true);

		/*
		* None of the below variables should be touched outside of program intialisation.
		*/

		// Holds pointers to all current IParser objects.
		extern IParser**	parserArray;

		// Holds how long each parser has been present for. Indexes match those in parserArray.
		extern Float32*		timerArray;

		// Maximum number of parsers that will be active at one time. 
		// Needs to be set before Initialise is run as this determines the size of the above arrays.
		extern UInt16		maxNumParsers;

		// Default time delay, in seconds, before automatically removing a parser.
		// Defaults to 30 seconds.
		extern UInt16		timeDelay;
	}
}

#endif // __PAPYRUS_FILEPARSER_H__