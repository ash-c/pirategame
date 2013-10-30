
#pragma once

#ifndef __PAPYRUS_LOGGER_LOGTOFILE_H__
#define __PAPYRUS_LOGGER_LOGTOFILE_H__

// Library Includes
#include <SDL_ttf.h>

// Local Includes
#include "ilogtype.h"

namespace Papyrus
{
	namespace Logger
	{
		class CLogToScreen : public ILogTarget
		{
			// Member Functions
		public:
			// Default Constructor
			CLogToScreen();

			// Default Destructor
			virtual ~CLogToScreen();

			virtual Bool		Initialise();
			virtual Bool		ShutDown();

			virtual void		Process(Float32 _fDelta);
			virtual void		Render();

			virtual void		Write(Int8* _msg);

			// Member Variables
		private:
			static const Int32	SM_MAXTRACKED = 10;
			static const Int32	SM_MAXWRITE = 15;

			TTF_Font*			m_font;
			SDL_Surface*		m_surface;
			SDL_Texture*		m_texture;
			SDL_Rect			m_pos;
			SDL_Colour			m_col;
			Int8*				m_trackedText;
			Int8*				m_displayText;
		};
	}
}

#endif // __PAPYRUS_LOGGER_LOGTOFILE_H__