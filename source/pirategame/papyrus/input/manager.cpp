
// Library Includes

// Local Includes
#include "manager.h"

using namespace Papyrus;

Input::CInputManager::CInputManager()
	: m_observers(0)
{
}

Input::CInputManager::~CInputManager()
{
	for (Int16 i = 0; i < MAX_OBSERVERS; ++i)
	{
		m_observers[i] = 0;
	}

	CLEANARRAY(m_observers);
}

Bool Input::CInputManager::Initialise()
{
	m_observers = new IInputObserver*[MAX_OBSERVERS];
	SDL_memset(m_observers, 0, sizeof(IInputObserver*) * MAX_BUFFER);

	return true;
}

void Input::CInputManager::Process(Float32 _delta)
{
}

Bool Input::CInputManager::Register(IInputObserver* _obs)
{
	return true;
}