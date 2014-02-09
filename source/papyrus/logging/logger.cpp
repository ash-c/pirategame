
// Library Includes

// Local Includes
#include "logger.h"
#include "logtoscreen.h"
#include "logtofile.h"
#include "logconsole.h"

using namespace Papyrus;

Logger::ILogType*	Logger::logTargets[MAX_TYPE];
lua_State*			Logger::luaState = 0;

Bool Logger::Initialise()
{
#ifdef _DEBUG
	CREATEPOINTER(Logger::logTargets[LOG_TO_SCREEN], CLogToScreen);
	VALIDATE(Logger::logTargets[LOG_TO_SCREEN]->Initialise());
	Logger::logTargets[LOG_TO_SCREEN]->AddRef();

	CREATEPOINTER(Logger::logTargets[LOG_TO_CONSOLE], CLogConsole);
	VALIDATE(Logger::logTargets[LOG_TO_CONSOLE]->Initialise());
	Logger::logTargets[LOG_TO_CONSOLE]->AddRef();
#endif // _DEBUG

	Logger::luaState = lua_open();
	luaL_openlibs(Logger::luaState);

	lua_register(Logger::luaState, "ToggleScreenDebug", ToggleScreenDebug);
	lua_register(Logger::luaState, "ToggleConsole", ToggleConsole);
	lua_register(Logger::luaState, "TestFunction", TestFunction);
	
	return true;
}

Bool Logger::ShutDown()
{
	lua_close(Logger::luaState);

	PY_DELETE_RELEASE(Logger::logTargets[LOG_TO_SCREEN]);
	PY_DELETE_RELEASE(Logger::logTargets[LOG_TO_FILE]);
	PY_DELETE_RELEASE(Logger::logTargets[LOG_TO_CONSOLE]);
	
	return true;
}

Bool Logger::InitFile(const Int8* _path)
{
	// Clean existing file logging if present.
	PY_DELETE_RELEASE(Logger::logTargets[LOG_TO_FILE]);

	Logger::logTargets[LOG_TO_FILE] = new CLogToFile();
	VALIDATE(Logger::logTargets[LOG_TO_FILE]->Initialise(_path));
	Logger::logTargets[LOG_TO_FILE]->AddRef();

	return true;
}

void Logger::Process(Float32 _delta)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->Process(_delta);
	Logger::logTargets[LOG_TO_CONSOLE]->Process(_delta);
#endif // _DEBUG
}

void Logger::Render()
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->Render();
	Logger::logTargets[LOG_TO_CONSOLE]->Render();
#endif // _DEBUG
}

void Logger::Write(Int8* _format, ...)
{
	va_list args;
	va_start(args, _format);

	Int8* text = new Int8[MAX_BUFFER];
	SDL_vsnprintf(text, MAX_BUFFER, _format, args);
	Logger::logTargets[LOG_TO_FILE]->Write(text);
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->Write(text);
	Logger::logTargets[LOG_TO_CONSOLE]->Write(text);
#endif // _DEBUG
	CLEANARRAY(text);
}

void Logger::WriteToScreen(Int8* _format, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args, _format);

	Int8* text = new Int8[MAX_BUFFER];
	SDL_vsnprintf(text, MAX_BUFFER, _format, args);
	Logger::logTargets[LOG_TO_SCREEN]->Write(text);
	CLEANARRAY(text);
#endif // _DEBUG
}

void Logger::WriteToConsole(Int8* _format, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args, _format);

	Int8* text = new Int8[MAX_BUFFER];
	SDL_vsnprintf(text, MAX_BUFFER, _format, args);
	Logger::logTargets[LOG_TO_CONSOLE]->Write(text);
	CLEANARRAY(text);
#endif // _DEBUG
}

void Logger::WriteToFile(Int8* _format, ...)
{
	if (0 != Logger::logTargets[LOG_TO_FILE])
	{
		va_list args;
		va_start(args, _format);

		Int8* text = new Int8[MAX_BUFFER];
		SDL_vsnprintf(text, MAX_BUFFER, _format, args);
		Logger::logTargets[LOG_TO_FILE]->Write(text);
		CLEANARRAY(text);
	}
}

Int32 Logger::ToggleConsole(lua_State *L)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_CONSOLE]->Toggle();
#endif // _DEBUG
	return 0;
}

Int32 Logger::ToggleScreenDebug(lua_State *L)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->Toggle();
#endif // _DEBUG
	return 0;
}

void Logger::TrackValue(VECTOR4* _v4, const Int8* _tag)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->TrackValue((void*)(_v4), TRACKTYPE_VECTOR4, _tag);
#endif // _DEBUG
}

void Logger::TrackValue(VECTOR3* _v3, const Int8* _tag)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->TrackValue((void*)(_v3), TRACKTYPE_VECTOR3, _tag);
#endif // _DEBUG
}

void Logger::TrackValue(VECTOR2* _v2, const Int8* _tag)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->TrackValue((void*)(_v2), TRACKTYPE_VECTOR2, _tag);
#endif // _DEBUG
}

void Logger::TrackValue(Float32* _f, const Int8* _tag)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->TrackValue((void*)(_f), TRACKTYPE_FLOAT, _tag);
#endif // _DEBUG
}

void Logger::TrackValue(Int32* _i, const Int8* _tag)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->TrackValue((void*)(_i), TRACKTYPE_INT, _tag);
#endif // _DEBUG
}

void Logger::TrackValue(UInt32* _u, const Int8* _tag)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->TrackValue((void*)(_u), TRACKTYPE_UINT, _tag);
#endif // _DEBUG
}

void Logger::TrackValue(Int8* _c, const Int8* _tag)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->TrackValue((void*)(_c), TRACKTYPE_CHAR, _tag);
#endif // _DEBUG
}

void Logger::TrackValue(Bool* _b, const Int8* _tag)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->TrackValue((void*)(_b), TRACKTYPE_BOOL, _tag);
#endif // _DEBUG
}

void Logger::StopTracking(const Int8* _tag)
{
#ifdef _DEBUG
	Logger::logTargets[LOG_TO_SCREEN]->StopTracking(_tag);
#endif // _DEBUG
}

Bool Logger::LogSDLError(const Int32 _code, const Int8* _msg)
{
	if (0 != _code)
	{
		Logger::WriteToFile("%s failed: %s", _msg, SDL_GetError());
		assert(!_code && _msg && "See log file in data/papyrus/");
		return false;
	}

	return true;
}

void Logger::SendInputToConsole(SDL_Event _e)
{
	Logger::logTargets[LOG_TO_CONSOLE]->Input(_e);
}

Int32 Logger::TestFunction(lua_State *L)
{
	Int32 i = static_cast<Int32>(lua_tonumber(L, 1));
	size_t length = MAX_BUFFER;
	const Int8* s = 0;
	s = lua_tolstring(L, 2, &length);
	Bool b = lua_toboolean(L, 3) == 1;

	Logger::WriteToConsole("Passed parameters: int - %i, string - %s, bool - %s", i, s, (b ? "true" : "false"));

	return 0;
}