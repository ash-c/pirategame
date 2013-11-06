
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

			virtual void Input(SDL_Event _e);

		private:
			typedef struct _TInputStore
			{
				Int8			buffer[MAX_BUFFER];
				Int16			numChars;

				_TInputStore()
				{
					clear();
				}

				void push(Int8 _c)
				{
					if (numChars == MAX_BUFFER) return;

					buffer[numChars] = _c;
					++numChars;
				}

				Bool isEmpty()
				{
					return 0 == numChars;
				}

				void clear()
				{
					SDL_memset(buffer, 0, sizeof(Int8) * MAX_BUFFER);
					numChars = 0;
				}
			} TInputStore;

			// Member Variables
		private:
			static const Int16	SM_MAXDISPLAY = 20;
			static const Int16	SM_DEFAULT_X = 10;
			static const Int16	SM_FONTSIZE = 18;

			TTF_Font*			m_font;
			SDL_Surface*		m_surface;
			SDL_Texture*		m_texture;
			SDL_Rect			m_pos;
			SDL_Rect			m_textPos;
			SDL_Colour			m_col;
			TInputStore			m_input;
			Int32				m_height;
			Int32				m_width;
			Int8*				m_displayText[SM_MAXDISPLAY];
			Bool				m_active;
		};
	}
}

#endif // __PAPYRUS_LOGGER_LOGCONSOLE_H__