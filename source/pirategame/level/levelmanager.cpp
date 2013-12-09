
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
	PY_DELETE_RELEASE(m_currLevel);

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
	CLEANDELETE(m_currLevel);
	m_currLevel = new CLevel;
	VALIDATE(m_currLevel->Initialise("data/levels/1.json"));
	m_currLevel->AddRef();

	return true;
}