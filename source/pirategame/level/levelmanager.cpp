
// Library Includes

// Local Includes
#include "levelmanager.h"
#include "level.h"

CLevelManager::CLevelManager()
	: m_currLevel(0)
{
}

CLevelManager::~CLevelManager()
{
}

Bool CLevelManager::Initialise()
{
	return true;
}

Bool CLevelManager::ShutDown()
{
	CLEANDELETE(m_currLevel);

	return true;
}

void CLevelManager::Process(Float32 _delta)
{
	if (0 != m_currLevel)
	{
		m_currLevel->Process(_delta);
	}
}

void CLevelManager::Render()
{
	if (0 != m_currLevel)
	{
		m_currLevel->Render();
	}
}

Bool CLevelManager::LoadLevel(Int32 _lvl)
{
	return true;
}