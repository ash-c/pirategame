

// Library Includes
#include <SDL.h>

// Local Includes
#include "../logging/logger.h"

// This Include
#include "timer.h"

// Implementation

Papyrus::Timer::CTimer::CTimer()
	: m_startTicks(0.0f)
	, m_pausedTicks(0.0f)
	, m_delta(0.0f)
	, m_timePassed(0.0f)
	, m_FPS(0)
	, m_maxFPS(0)
	, m_started(false)
	, m_paused(false)
{

}

Papyrus::Timer::CTimer::~CTimer()
{

}

Bool Papyrus::Timer::CTimer::Initialise()
{
	Papyrus::Logger::TrackValue((Float32*)(&m_FPS), "FPS");
	Papyrus::Logger::Write("Timer initialised", NULL);
		
	return true;
}

void Papyrus::Timer::CTimer::Start()
{
	m_started = true;
	m_paused = false;
	m_startTicks = static_cast<Float32>(SDL_GetTicks());
}

void Papyrus::Timer::CTimer::Stop()
{
	m_started = false;
	m_paused = false;
}

void Papyrus::Timer::CTimer::Pause()
{
	if (m_started && !m_paused)
	{
		m_paused = true;
		m_pausedTicks = SDL_GetTicks() - m_startTicks;
	}
}

void Papyrus::Timer::CTimer::UnPause()
{
	if (m_paused)
	{
		m_paused = false;
		m_startTicks = SDL_GetTicks() - m_pausedTicks;
		m_pausedTicks = 0;
	}
}

Float32 Papyrus::Timer::CTimer::Restart()
{
	Float32 elapsedTicks = Ticks();
	Start();
	return elapsedTicks;
}

Float32 Papyrus::Timer::CTimer::Ticks()
{
	if (m_started)
	{
		if (m_paused)
			return m_pausedTicks;
		else
		{
			m_delta = (SDL_GetTicks() - m_startTicks) / 1000.0f;
			m_timePassed += m_delta;
			if (m_timePassed <= 1.0f)
			{
				++m_maxFPS;
			}
			else
			{
				m_timePassed = 0.0f;
				m_FPS = static_cast<Float32>(m_maxFPS);
				m_maxFPS = 0;
			}
			return m_delta;
		}
	}
	return 0;
}

Float32 Papyrus::Timer::CTimer::GetFPS() const
{
	return m_FPS;
}

Bool Papyrus::Timer::CTimer::Started() const
{
	return m_started;
}

Bool Papyrus::Timer::CTimer::Paused() const
{
	return m_paused;
}