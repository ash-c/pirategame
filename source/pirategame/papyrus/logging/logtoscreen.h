
#pragma once

#ifndef __PAPYRUS_LOGGER_LOGTOSCREEN_H__
#define __PAPYRUS_LOGGER_LOGTOSCREEN_H__

// Library Includes
#include <SDL_ttf.h>

// Local Includes
#include "ilogtype.h"

namespace Papyrus
{
	namespace Logger
	{
		typedef struct _TTrackInfo
		{
			ETrackType		type;
			Int8*			tag;
			void*			value;
		} TTrackInfo;

		class CLogToScreen : public ILogType
		{
			// Member Functions
		public:
			// Default Constructor
			CLogToScreen();

			// Default Destructor
			virtual ~CLogToScreen();

			virtual Bool		Initialise(const Int8* _path = 0);
			virtual Bool		ShutDown();

			virtual void		Process(Float32 _fDelta);
			virtual void		Render();

			virtual void		Write(const Int8* _msg);

			virtual Bool		Toggle();

			virtual void		TrackValue(void* _value, ETrackType _type, const Int8* _tag);
			virtual void		StopTracking(const Int8* _tag);

			// Member Variables
		private:
			static const Int16	SM_MAXTRACKED = 10;
			static const Int16	SM_MAXTEMP = 15;
			static const Int16	SM_DEFAULT_X = 10;
			static const Int16	SM_DEFAULT_Y = 10;
			static const Int16	SM_TIMER = 3;
			static const Int16	SM_FONTSIZE = 18;

			TTF_Font*			m_font;
			SDL_Surface*		m_surface;
			SDL_Texture*		m_texture;
			SDL_Rect			m_pos;
			SDL_Colour			m_col;
			TTrackInfo*			m_trackedInfo[SM_MAXTRACKED];
			Float32				m_timer;
			Int8*				m_tempText[SM_MAXTEMP];
			Bool				m_active;
		};
	}
}

#endif // __PAPYRUS_LOGGER_LOGTOSCREEN_H__