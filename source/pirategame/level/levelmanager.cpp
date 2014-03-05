
// Library Includes

// Local Includes
#include "levelmanager.h"
#include "level.h"

CLevelManager::CLevelManager()
	: m_current(0)
	, m_numlevels(0)
	, m_currLevel(0)
	, m_finLevels(false)
{
}

CLevelManager::~CLevelManager()
{
}

Bool CLevelManager::Initialise(Int32 _numLevels, Int32 _currLevel)
{
	lua_register(Logger::luaState, "RestartLevel", RestartLevel);

	if (0 != _numLevels)
	{
		m_numlevels = _numLevels;
	}
	m_currLevel = _currLevel;
	m_finLevels = false;

	return true;
}

Bool CLevelManager::ShutDown()
{	
	PY_DELETE_RELEASE(m_current);

	return true;
}

void CLevelManager::Process(Float32 _delta)
{
	if (0 != m_currLevel)
	{
		m_current->Process(_delta);

		if (m_current->IsComplete())
		{
			++m_currLevel;

			if (m_currLevel <= m_numlevels)
			{
				Int8* temp = new Int8[MAX_BUFFER];
				SDL_snprintf(temp, MAX_BUFFER, "data/levels/%i.json", m_currLevel);
				if (!LoadLevel(temp))
				{
					Logger::Write("Failed to load level %s", temp);
				}
				CLEANARRAY(temp);
			}
			else
			{
				m_finLevels = true;
			}
		}
	}
}

void CLevelManager::Render()
{
	if (0 != m_currLevel)
	{
		m_current->Render();
	}
}

Bool CLevelManager::LoadLevel(Int8* _lvl)
{	
	PY_DELETE_RELEASE(m_current);

	CLevel* temp = new CLevel;
	if (!temp->Initialise(_lvl))
	{
		Logger::Write("Failed to load level %s", _lvl);
		CLEANDELETE(temp);
		return false;
	}

	m_current = temp;
	m_current->AddRef();

	return true;
}

Bool CLevelManager::ResetLevel()
{
	VALIDATE(m_current->Reset());
	return true;
}

Int32 CLevelManager::RestartLevel(lua_State* L)
{
	sm_pTheInstance->ResetLevel();

	return 0;
}