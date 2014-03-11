
// Library Includes

// Local Includes
#include "manager.h"
#include "../logging/logger.h"

using namespace Papyrus;

Input::CInputManager::CInputManager()
	: m_observers(0)
	, m_joystick(0)
	, m_controller(0)
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

	if (SDL_NumJoysticks() < 1)
	{
		Logger::Write("No controller connected");
	}
	else 
	{
		if (SDL_IsGameController(0))
		{
			m_controller = SDL_GameControllerOpen(0);
			assert(m_controller);
			m_joystick = SDL_GameControllerGetJoystick(m_controller);
			assert(m_joystick);
			Logger::Write("Controller connected");
		}
	}

	return true;
}

Bool Input::CInputManager::ShutDown()
{
	if (0 != m_controller)
	{
		SDL_GameControllerClose(m_controller);
		m_controller = 0;
	}

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
		if (e.type == SDL_CONTROLLERDEVICEREMOVED)
		{
			Logger::WriteToScreen("Controller removed");
			SDL_GameControllerClose(m_controller);
			m_controller = 0;
		}
		else if (e.type == SDL_CONTROLLERDEVICEADDED)
		{
			Logger::WriteToScreen("Controller added");
			m_controller = SDL_GameControllerOpen(0);
			assert(m_controller);
			m_joystick = SDL_GameControllerGetJoystick(m_controller);
			assert(m_joystick);
		}

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

Bool Input::CInputManager::DeRegister(IInputObserver* _obs)
{
	for (Int16 i = 0; i < MAX_OBSERVERS; ++i)
	{
		if (_obs == m_observers[i])
		{
			m_observers[i] = 0;
			return true;
		}
	}

	Logger::Write("Could not find input observer to deregister");
	return false;
}

Int32 Input::CInputManager::GetNumControllers()
{
	if (0 == m_controller) return 0;
	else return 1;
}