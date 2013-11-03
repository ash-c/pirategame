
#pragma once

#ifndef __PAPYRUS_LOGGER_LOGTOFILE_H__
#define __PAPYRUS_LOGGER_LOGTOFILE_H__

// Library Includes
#include <SDL.h>

// Local Includes
#include "ilogtype.h"

namespace Papyrus
{
	namespace Logger
	{
		class CLogToFile : public ILogType
		{
			// Member Functions
		public:
			// Default Constructor
			CLogToFile();

			// Default Destructor
			virtual ~CLogToFile();

			virtual Bool		Initialise(const Int8* _path = 0);
			virtual Bool		ShutDown();

			virtual void		Write(Int8* _msg);

			// Member Variables
		private:
			SDL_RWops*			m_file;
		};
	}
}

#endif // __PAPYRUS_LOGGER_LOGTOFILE_H__