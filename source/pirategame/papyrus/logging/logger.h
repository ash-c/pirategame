
#pragma once

#ifndef __PAPYURS_LOGGER_H__
#define __PAPYURS_LOGGER_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "ilogtype.h"

namespace Papyrus
{
	namespace Logger
	{
		typedef enum _ELOGGER_TYPES
		{
			INVALID_TYPE = INVALID_ID,
			LOG_TO_SCREEN,
			LOG_TO_FILE,
			LOG_TO_CONSOLE,
			MAX_TYPE
		} E_LOGGER_TYPES;

		/*
		* Initialises the logging system.
		*
		* @return	Returns true on successfull intialisation.
		*/
		Bool		Initialise();

		/*
		* Shuts down the logging system.
		*
		* @return	Returns true when successfully shut down.
		*/
		Bool		ShutDown();

		/*
		* Processes the screen logging, to remove values after a set time.
		*
		* @param	_fDelta			Time since last process.
		* @return	Returns void.
		*/
		void		Process(Float32 _fDelta);

		/*
		* Renders required logging elements to screen.
		*
		* @return	Returns void.
		*/
		void		Render();

		/*
		* Writes a message to all active logging devices.
		*
		* @param	_format			Format for the message to be logged. Like sprintf etc.
		* @param	...				Variable arguments.
		* @return	Returns void.
		*/
		void		Write(Int8* _format, ...);

		/*
		* Writes a message to the screen only.
		*
		* @param	_format			Format for the message to be logged. Like sprintf etc.
		* @param	...				Variable arguments.
		* @return	Returns void.
		*/
		void		WriteToScreen(Int8* _format, ...);

		/*
		* Writes a message to the console only.
		*
		* @param	_format			Format for the message to be logged. Like sprintf etc.
		* @param	...				Variable arguments.
		* @return	Returns void.
		*/
		void		WriteToConsole(Int8* _format, ...);

		/*
		* Writes a message to file only.
		*
		* @param	_format			Format for the message to be logged. Like sprintf etc.
		* @param	...				Variable arguments.
		* @return	Returns void.
		*/
		void		WriteToFile(Int8* _format, ...);

		void		TrackValue(VECTOR4* _v4, const Int8* _tag);
		void		TrackValue(VECTOR3* _v3, const Int8* _tag);
		void		TrackValue(Float32* _f, const Int8* _tag);
		void		TrackValue(Int32* _i, const Int8* _tag);
		void		TrackValue(UInt32* _u, const Int8* _tag);
		void		TrackValue(Int8* _c, const Int8* _tag);
		void		TrackValue(Bool* _b, const Int8* _tag);

		void		StopTracking(const Int8* _tag);

		extern ILogTarget** logTargets;
	}
}

#endif // __PAPYURS_LOGGER_H__