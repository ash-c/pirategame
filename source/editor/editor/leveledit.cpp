
// Library Includes

// Local Includes
#include "leveledit.h"
#include "level\level.h"
#include "level\tile.h"

#include "tool.h"
#include "tools\tiletool.h"

#include "../../papyrus/ui/ui.h"

CLevelEdit::CLevelEdit()
	: m_level(0)
	, m_toolContext(0)
	, m_activeTool(TOOL_TILE)
	, m_screenHeight(0)
	, m_screenWidth(0)
	, m_rightMouseDown(false)
	, m_leftMouseDown(false)
	, m_mouseMoved(false)
{
}

CLevelEdit::~CLevelEdit()
{
}

Bool CLevelEdit::Initialise()
{
	sm_pTheInstance = this;

	CREATEPOINTER(m_level, CLevel);
	assert(m_level);
	VALIDATE(m_level->Initialise("data/levels/editorNew.json"));
	//VALIDATE(m_level->Initialise("data/levels/1.json"));
	m_level->AddRef();

	m_screenHeight = Renderer::activeRenderer->GetHeight();
	m_screenWidth = Renderer::activeRenderer->GetWidth();
	m_cameraPos.y = static_cast<Float32>(LEVEL_HEIGHT - m_screenHeight);

	m_toolContext = UI::LoadInterface("data/interfaces/editorBuildContext.ini", false, true);
	assert(m_toolContext);
	m_toolContext->AddRef();

	VALIDATE(IEditor::Initialise());

	m_escMenu = UI::LoadInterface("data/interfaces/editorLevelMenu.ini");
	assert(m_escMenu);
	m_escMenu->AddRef();
	m_escMenu->Toggle();

	return true;
}

Bool CLevelEdit::ShutDown()
{
	PY_DELETE_RELEASE(m_level);
	PY_SAFE_RELEASE(m_toolContext);
	PY_SAFE_RELEASE(m_escMenu);

	return true;
}

void CLevelEdit::Process(Float32 _delta)
{
	if (0 != m_level) m_level->CameraPos(m_cameraPos);

	if (0.0f < m_tileDelay) m_tileDelay -= _delta;

	// Reset mouse moved state
	if (!SDL_HasEvent(SDL_MOUSEMOTION) && m_mouseMoved)
	{
		m_mouseMoved = false;
	}
}

void CLevelEdit::Render()
{
	if (0 != m_level) m_level->Render();
	if (0!= m_toolContext) m_toolContext->Render();

	if (m_escMenu->IsActive()) m_escMenu->Render();
}

Bool CLevelEdit::Save(Int8* _path)
{
	if (m_level->Save(_path))
	{
		Logger::Write("Saved level");
		return true;
	}
	else
	{
		Logger::Write("Save failed");
		return false;
	}
}

void CLevelEdit::Notify(SDL_Event* _e)
{
	if (SDL_MOUSEBUTTONDOWN == _e->type)
	{
		if (SDL_BUTTON_RIGHT == _e->button.button && !m_leftMouseDown)
		{
			m_rightMouseDown = true;
		}
		else if (SDL_BUTTON_LEFT == _e->button.button && !m_rightMouseDown)
		{
			if (!m_toolContext->IsActive())
			{
				m_leftMouseDown = true;
				VECTOR2 tilePos(static_cast<Float32>(_e->button.x), static_cast<Float32>(_e->button.y));
				VECTOR2 newPos;
				newPos.y = tilePos.y + m_cameraPos.y;
				newPos.x = tilePos.x - m_cameraPos.x;

				// Place tool.
				if (TOOL_TILE == m_activeTool)
				{
					m_level->AddTile(newPos);
				}
				else if (TOOL_WATER == m_activeTool)
				{
					m_level->AddWater(newPos);
				}
				else if (TOOL_COIN == m_activeTool)
				{
					m_level->AddCoin(newPos);
				}
				else if (TOOL_REMOVE == m_activeTool)
				{
					if (!m_level->RemoveTile(newPos))
					{
						if (!m_level->RemoveEnemy(newPos))
						{
							if (!m_level->RemoveMovingPlatform(newPos))
							{
								m_level->RemoveCoin(newPos);
							}
						}
					}
				}
				else if (TOOL_BASIC_ENEMY == m_activeTool)
				{
					m_level->AddEnemy(newPos, Physics::EType::TYPE_BASIC_ENEMY);
				}
				else if (TOOL_PLAYER_START == m_activeTool)
				{
					m_level->SetPlayerStart(newPos);
				}
				else if (TOOL_PLATFORM == m_activeTool)
				{
					m_level->AddMovingPlatform(newPos);
				}
			}
		}
	}
	else if (SDL_MOUSEBUTTONUP == _e->type)
	{
		if (SDL_BUTTON_RIGHT == _e->button.button)
		{	
			if (!m_mouseMoved) 
			{
				Logger::Write("right click menu open");
				m_toolContext->Show(&VECTOR2(static_cast<Float32>(_e->button.x), static_cast<Float32>(_e->button.y)));
			}
			m_rightMouseDown = false;
		}
		if (SDL_BUTTON_LEFT == _e->button.button)
		{
			m_toolContext->Hide();
			m_leftMouseDown = false;
		}
	}
	else if (SDL_MOUSEMOTION == _e->type)
	{
		m_mouseMoved = true;
		if (m_rightMouseDown)
		{
			m_cameraPos.x += _e->motion.xrel;
			m_cameraPos.y -= _e->motion.yrel;

			if (m_cameraPos.y < 0.0f) m_cameraPos.y = 0.0f;
			if (m_cameraPos.y > (LEVEL_HEIGHT - m_screenHeight)) m_cameraPos.y = static_cast<Float32>(LEVEL_HEIGHT - m_screenHeight);

			if (m_cameraPos.x > 0.0f) m_cameraPos.x = 0.0f; 
			if (m_cameraPos.x < -(LEVEL_WIDTH - m_screenWidth)) m_cameraPos.x = -static_cast<Float32>(LEVEL_WIDTH - m_screenWidth);
		}
		else if (m_leftMouseDown)
		{
			VECTOR2 tilePos(static_cast<Float32>(_e->button.x), static_cast<Float32>(_e->button.y));
			VECTOR2 newPos;
			newPos.y = tilePos.y + m_cameraPos.y;
			newPos.x = tilePos.x - m_cameraPos.x;

			// Place tool.
			if (TOOL_TILE == m_activeTool)
			{
				m_level->AddTile(newPos);
			}
			else if (TOOL_WATER == m_activeTool)
			{
				m_level->AddWater(newPos);
			}
			else if (TOOL_REMOVE == m_activeTool)
			{
				if (!m_level->RemoveTile(newPos))
				{
					if (!m_level->RemoveEnemy(newPos))
					{
						m_level->RemoveMovingPlatform(newPos);
					}
				}
			}
			else if (TOOL_PLATFORM == m_activeTool)
			{
				m_level->AddMovingPlatform(newPos);
			}
		}
	}
}

void CLevelEdit::SetTool(Int32 _new)
{
	m_activeTool = static_cast<EToolType>(_new);
}

void CLevelEdit::Load(const Int8* _path)
{
	UI::LoadingScreen();
	CLevel* temp = new CLevel;
	if (!temp->Initialise((Int8*)_path))
	{
		Logger::Write("Failed to load level %s", _path);
		CLEANDELETE(temp);
		return;
	}

	PY_DELETE_RELEASE(m_level);
	m_level = temp;
	m_level->AddRef();
}

void CLevelEdit::ToggleEscMenu()
{
	m_escMenu->Toggle();
}
