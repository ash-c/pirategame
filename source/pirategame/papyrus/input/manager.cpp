
// Library Includes

// Local Includes
#include "manager.h"
#include "../logging/logger.h"

using namespace Papyrus;

Input::CInputManager::CInputManager()
	: m_observers(0)
	, m_joystick(0)
{
}

Input::CInputManager::~CInputManager()
{
}

Bool Input::CInputManager::Initialise()
{
	m_observers = new IInputObserver*[MAX_OBSERVERS];
	assert(m_observers);
	SDL_memset(m_observers, 0, sizeof(IInputObserver*) * MAX_OBSERVERS);

	m_joystick = SDL_JoystickOpen(0);
	if (0 == m_joystick)
	{
		Logger::Write("No controller connected");
	}
	else 
	{
		Logger::Write("Controller connected");
	}

	return true;
}

Bool Input::CInputManager::ShutDown()
{
	SDL_JoystickClose(m_joystick);

	for (Int16 i = 0; i < MAX_OBSERVERS; ++i)
	{
		m_observers[i] = 0;
	}

	CLEANARRAY(m_observers);

	return true;
}

void Input::CInputManager::Process(Float32 _delta)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{	
		for (Int16 i = 0; i < MAX_OBSERVERS; ++i)
		{
			if (0 != m_observers[i])
			{
				m_observers[i]->Notify(&e);
			}
		}
	}
}

Bool Input::CInputManager::Register(IInputObserver* _obs)
{
	for (Int16 i = 0; i < MAX_OBSERVERS; ++i)
	{
		if (0 == m_observers[i])
		{
			m_observers[i] = _obs;
			return true;
		}
	}

	Logger::Write("Input Observer failed to register, reached max.");
	return false;
}