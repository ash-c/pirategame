
// Library Includes

// Local Includes
#include "logger.h"
#include "logtoscreen.h"

using namespace Papyrus;

Logger::ILogTarget** Logger::logTargets = 0;

Bool Logger::Initialise()
{
	Logger::logTargets = new ILogTarget*[MAX_TYPE];

	Logger::logTargets[LOG_TO_SCREEN] = new CLogToScreen();
	assert(Logger::logTargets[LOG_TO_SCREEN]);
	VALIDATE(Logger::logTargets[LOG_TO_SCREEN]->Initialise());
	Logger::logTargets[LOG_TO_SCREEN]->AddRef();
	
	return true;
}

Bool Logger::ShutDown()
{
	Logger::logTargets[LOG_TO_SCREEN]->Release();
	Logger::logTargets[LOG_TO_SCREEN]->ShutDown();
	return true;
}

void Logger::Process(Float32 _fDelta)
{
	Logger::logTargets[LOG_TO_SCREEN]->Process(_fDelta);
}

void Logger::Render()
{
	Logger::logTargets[LOG_TO_SCREEN]->Render();
}

void Logger::Write(Int8* _format, ...)
{
	Logger::logTargets[LOG_TO_SCREEN]->Write(_format);
}

void Logger::WriteToScreen(Int8* _format, ...)
{
}

void Logger::WriteToConsole(Int8* _format, ...)
{
}

void Logger::WriteToFile(Int8* _format, ...)
{
}

void Logger::TrackValue(VECTOR4* _v4, const Int8* _tag)
{
}

void Logger::TrackValue(VECTOR3* _v3, const Int8* _tag)
{
}

void Logger::TrackValue(Float32* _f, const Int8* _tag)
{
}

void Logger::TrackValue(Int32* _i, const Int8* _tag)
{
}

void Logger::TrackValue(UInt32* _u, const Int8* _tag)
{
}

void Logger::TrackValue(Int8* _c, const Int8* _tag)
{
}

void Logger::TrackValue(Bool* _b, const Int8* _tag)
{
}

void Logger::StopTracking(const Int8* _tag)
{
}