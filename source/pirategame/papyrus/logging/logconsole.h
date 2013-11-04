
#pragma once

#ifndef __PAPYRUS_LOGGER_LOGCONSOLE_H__
#define __PAPYRUS_LOGGER_LOGCONSOLE_H__

// Library Includes
#include <SDL_ttf.h>

// Local Includes
#include "ilogtype.h"

namespace Papyrus
{
	namespace Logger
	{
		class CLogConsole : public ILogType
		{
			// Member Functions
		public:
			// Default Constructor
			CLogConsole();

			// Default Destructor
			virtual ~CLogConsole();

			virtual Bool		Initialise(const Int8* _path = 0);
			virtual Bool		ShutDown();

			virtual void		Process(Float32 _fDelta);
			virtual void		Render();

			virtual void		Write(Int8* _msg);

			virtual Bool		Toggle();

			// Member Variables
		private:
			static const Int32	SM_MAXDISPLAY = 20;
			static const Int32	SM_DEFAULT_X = 10;

			TTF_Font*			m_font;
			SDL_Surface*		m_surface;
			SDL_Texture*		m_texture;
			SDL_Rect			m_pos;
			SDL_Rect			m_textPos;
			SDL_Colour			m_col;
			Int8*				m_displayText[SM_MAXDISPLAY];
			Bool				m_active;
		};
	}
}

#endif // __PAPYRUS_LOGGER_LOGCONSOLE_H__