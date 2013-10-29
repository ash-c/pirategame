

#pragma once

#ifndef __PAPYRUS_TIMER_H__
#define __PAPYRUS_TIMER_H__

// Library Includes
#include <ctime>

// Local Includes
#include "../../defines.h"

namespace Papyrus
{
	namespace Timer
	{
		class CTimer
		{
			// Member Functions
		public:
			// Default Constructor
			CTimer();
			// Default Destructor
			~CTimer();

			/*
			*
			* Initialises the clock.
			*
			* @return	Returns true if successful.
			*/
			Bool Initialise();

			void Start();

			void Stop();

			void Pause();

			void UnPause();

			Float32 Restart();

			Float32 Ticks();

			Int32 GetFPS() const;

			Bool Started() const;

			Bool Paused() const;
	
		private:
			CTimer(const CTimer& _rhs);
			CTimer& operator = (const CTimer& _rhs);

			// Member Variables
		private:	
			Float32				m_startTicks;
			Float32				m_pausedTicks;
			Float32				m_delta;
			Float32				m_timePassed;
			Int32				m_FPS;
			Int32				m_maxFPS;
			Bool				m_started;
			Bool				m_paused;
		};
	}
}
#endif //__PAPYRUS_TIMER_H__
