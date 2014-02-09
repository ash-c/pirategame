
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
	return true;
}

void Logger::CLogToFile::Write(const Int8* _msg)
{
	size_t length = SDL_strlen(_msg) + 2;
	Int8* newMsg = new Int8[length];
	SDL_snprintf(newMsg, length, "%s\n", _msg);
	length = SDL_strlen(newMsg);
	if (SDL_RWwrite(m_file, newMsg, 1, length) != length)
	{
		assert("Failed to write to file");
	}
	CLEANARRAY(newMsg);
}