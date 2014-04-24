
// Library Includes

// Local Includes
#include "levelmanager.h"
#include "level.h"

#include "../game.h"

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
	if (0 != _numLevels)
	{
		lua_register(Logger::luaState, "RestartLevel", RestartLevel);
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
			// save score from completed level
			Int32 score = 0;
			FileParser::IParser* save = FileParser::LoadFile("data/xml/saveState.xml");
			assert(save);
			save->AddRef();

			Int8 text[MAX_BUFFER];
			SDL_snprintf(text, MAX_BUFFER, "%iScore", m_currLevel);
			save->AddValue(text, m_current->GetScore());
			save->Save();
			save->Release();

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
	if (0 != m_current)
	{
		m_current->Render();
	}
}

Bool CLevelManager::LoadLevel(Int8* _lvl)
{	
	Core::Pause();
	PY_DELETE_RELEASE(m_current);

	CLevel* temp = new CLevel;
	if (!temp->Initialise(_lvl, m_currLevel))
	{
		Logger::Write("Failed to load level %s", _lvl);
		CLEANDELETE(temp);
		Core::Pause();
		return false;
	}

	m_current = temp;
	m_current->AddRef();

	Core::Pause();

	return true;
}

Bool CLevelManager::ResetLevel()
{
	Core::Pause();
	VALIDATE(m_current->Reset());
	Core::Pause();
	return true;
}

Int32 CLevelManager::RestartLevel(lua_State* L)
{
	sm_pTheInstance->ResetLevel();
	if (CGame::GetInstance().IsPaused())
	{
		CGame::PauseGame(0);
	}

	return 0;
}