
#pragma once

#ifndef __PAPYURS_LOGGER_H__
#define __PAPYURS_LOGGER_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "ilogtype.h"

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

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
		* Initialises a file to log to. If file logging is already active, the pre-existing one is shut down.
		*
		* @param	_path			File to log to.
		* @return	Returns true if successfully initialised, false otherwise.
		*/
		Bool		InitFile(const Int8* _path);

		/*
		* Processes the screen logging, to remove values after a set time.
		*
		* @param	_fDelta			Time since last process.
		* @return	Returns void.
		*/
		void		Process(Float32 _delta);

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

		/*
		* Turns the console on or off.
		*
		* @param	L				The luastate variable.
		* @return	Returns the number of variables on the lua stack when the function finishes.
		*/
		Int32		ToggleConsole(lua_State *L);

		/*
		* Turns screen debug display on or off. Called via lua scripting.
		*
		* @param	L				The luastate variable.
		* @return	Returns the number of variables on the lua stack when the function finishes.
		*/
		Int32		ToggleScreenDebug(lua_State *L);

		/*
		* Tracks the given value, and displays it permanently on screen.
		*
		* @param	_v4				Pointer to the VECTOR4 value to track.
		* @param	_tag			String to display what the value is tracking.
		* @return	Returns void.
		*/
		void		TrackValue(VECTOR4* _v4, const Int8* _tag);

		/*
		* Tracks the given value, and displays it permanently on screen.
		*
		* @param	_v4				Pointer to the VECTOR3 value to track.
		* @param	_tag			String to display what the value is tracking.
		* @return	Returns void.
		*/
		void		TrackValue(VECTOR3* _v3, const Int8* _tag);

		/*
		* Tracks the given value, and displays it permanently on screen.
		*
		* @param	_v4				Pointer to the Float32 value to track.
		* @param	_tag			String to display what the value is tracking.
		* @return	Returns void.
		*/
		void		TrackValue(Float32* _f, const Int8* _tag);

		/*
		* Tracks the given value, and displays it permanently on screen.
		*
		* @param	_v4				Pointer to the Int32 value to track.
		* @param	_tag			String to display what the value is tracking.
		* @return	Returns void.
		*/
		void		TrackValue(Int32* _i, const Int8* _tag);

		/*
		* Tracks the given value, and displays it permanently on screen.
		*
		* @param	_v4				Pointer to the UInt32 value to track.
		* @param	_tag			String to display what the value is tracking.
		* @return	Returns void.
		*/
		void		TrackValue(UInt32* _u, const Int8* _tag);

		/*
		* Tracks the given value, and displays it permanently on screen.
		*
		* @param	_v4				Pointer to the Int8 value to track.
		* @param	_tag			String to display what the value is tracking.
		* @return	Returns void.
		*/
		void		TrackValue(Int8* _c, const Int8* _tag);

		/*
		* Tracks the given value, and displays it permanently on screen.
		*
		* @param	_v4				Pointer to the Bool value to track.
		* @param	_tag			String to display what the value is tracking.
		* @return	Returns void.
		*/
		void		TrackValue(Bool* _b, const Int8* _tag);

		/*
		* Stops tracking a value.
		*
		* @param	_tag			Tag of the value to stop tracking.
		* @return	Returns void.
		*/
		void		StopTracking(const Int8* _tag);

		/*
		* Logs an SDL error.
		*
		* @param	_msg			Message to associate with the error.
		* @return	Returns void.
		*/
		Bool		LogSDLError(const Int32 _code, const Int8* _msg);
		
		/*
		* Function for testing parameters through console input.
		*
		* @param	L				The luastate variable.
		* @return	Returns the number of variables on the lua stack when the function finishes.
		*/
		Int32		TestFunction(lua_State *L);

		void SendInputToConsole(SDL_Event _e);

		// Array that holds pointers to the active log targets.
		extern ILogType*	logTargets[MAX_TYPE];

		extern lua_State*	luaState;
	}
}

// Some macros to assist with logging
#define PY_WRITETOFILE(text) Papyrus::Logger::WriteToFile(text, NULL);
#define PY_WRITETOSCREEN(text) Papyrus::Logger::WriteToScreen(text, NULL);
#define PY_WRITETOCONSOLE(text) Papyrus::Logger::WriteToConsole(text, NULL);

#define PY_SDLASSERT(code, msg) Papyrus::Logger::LogSDLError(code, msg)

#endif // __PAPYURS_LOGGER_H__