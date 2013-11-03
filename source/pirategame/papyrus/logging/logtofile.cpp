
// Library Includes

// Local Includes
#include "logtofile.h"

using namespace Papyrus;

Logger::CLogToFile::CLogToFile()
	: m_file(0)
{
}

Logger::CLogToFile::~CLogToFile()
{
}

Bool Logger::CLogToFile::Initialise(const Int8* _path)
{
	assert(_path);
	m_file = SDL_RWFromFile(_path, "w");
	assert(m_file);
	return true;
}

Bool Logger::CLogToFile::ShutDown()
{
	VALIDATE(SDL_RWclose(m_file));
	assert(m_file);
	return true;
}

void Logger::CLogToFile::Write(Int8* _msg)
{
	size_t length = SDL_strlen(_msg);
	if (SDL_RWwrite(m_file, _msg, 1, length) != length)
	{
#pragma todo("Error logging here")
	}
}