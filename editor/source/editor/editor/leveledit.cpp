
// Library Includes

// Local Includes
#include "leveledit.h"
#include "level\level.h"
#include "level\tile.h"

#include "tool.h"
#include "tools\tiletool.h"

CLevelEdit::CLevelEdit()
	: m_level(0)
	, m_activeTool(INVALID_TOOL)
	, m_screenHeight(0)
	, m_screenWidth(0)
	, m_rightMouseDown(false)
	, m_leftMouseDown(false)
{
}

CLevelEdit::~CLevelEdit()
{
}

Bool CLevelEdit::Initialise()
{
	CREATEPOINTER(m_level, CLevel);
	assert(m_level);
	VALIDATE(m_level->Initialise("test.json"));
	m_level->AddRef();

	m_screenHeight = Renderer::activeRenderer->GetHeight();
	m_screenWidth = Renderer::activeRenderer->GetWidth();

	SDL_memset(m_tools, 0, sizeof(ITool*) * MAX_TOOL);

	// Initialise tools.
	CREATEPOINTER(m_tools[TOOL_TILE], CToolTile);
	assert(m_tools[TOOL_TILE]);
	VALIDATE(m_tools[TOOL_TILE]->Initialise());

	IEditor::Initialise();

	return true;
}

Bool CLevelEdit::ShutDown()
{
	PY_DELETE_RELEASE(m_level);

	m_tools[m_activeTool]->Release();

	for (UInt16 i = 0; i < MAX_TOOL; ++i)
	{
		if (0 != m_tools[i]) m_tools[i]->ShutDown();
		CLEANDELETE(m_tools[i]);
	}

	return true;
}

void CLevelEdit::Process(Float32 _delta)
{
	if (0 != m_level) m_level->CameraPos(m_cameraPos);

	if (0.0f < m_tileDelay) m_tileDelay -= _delta;
}

void CLevelEdit::Render()
{
	if (0 != m_level) m_level->Render();
}

Bool CLevelEdit::Save()
{
	if (m_level->Save())
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
		if (SDL_BUTTON_RIGHT == _e->button.button)
		{
			m_rightMouseDown = true;
		}
		else if (SDL_BUTTON_LEFT == _e->button.button)
		{
			m_leftMouseDown = true;
			if (INVALID_TOOL != m_activeTool && MAX_TOOL > m_activeTool)
			{
				VECTOR2 tilePos(static_cast<Float32>(_e->button.x), static_cast<Float32>(_e->button.y));

				// Place tool.
				if (m_tools[m_activeTool]->AddToLevel(m_level, tilePos - m_cameraPos))
				{
					//Logger::Write("Tool placed at (%.2f, %.2f)", tilePos.x, tilePos.y);
				}
				else
				{
					//Logger::Write("Error, failed to place tool %i", m_activeTool);
				}
			}
			else 
			{
				m_activeTool = TOOL_TILE;
				m_tools[m_activeTool]->AddRef();
			}
		}
	}
	else if (SDL_MOUSEBUTTONUP == _e->type)
	{
		if (SDL_BUTTON_RIGHT == _e->button.button)
		{
			m_rightMouseDown = false;
		}
		if (SDL_BUTTON_LEFT == _e->button.button)
		{
			m_leftMouseDown = false;
		}
	}
	else if (SDL_MOUSEMOTION == _e->type)
	{
		if (m_rightMouseDown)
		{
			m_cameraPos.x += _e->motion.xrel;
			m_cameraPos.y += _e->motion.yrel;

			if (m_cameraPos.y < 0.0f) m_cameraPos.y = 0.0f;
			if (m_cameraPos.y > (LEVEL_HEIGHT - m_screenHeight)) m_cameraPos.y = static_cast<Float32>(LEVEL_HEIGHT - m_screenHeight);

			if (m_cameraPos.x > 0.0f) m_cameraPos.x = 0.0f; 
			if (m_cameraPos.x < -(LEVEL_WIDTH - m_screenWidth)) m_cameraPos.x = -static_cast<Float32>(LEVEL_WIDTH - m_screenWidth);
		}
		if (m_leftMouseDown)
		{
			if (INVALID_TOOL != m_activeTool && MAX_TOOL > m_activeTool)
			{
				VECTOR2 tilePos(static_cast<Float32>(_e->button.x), static_cast<Float32>(_e->button.y));

				// Place tool.
				if (m_tools[m_activeTool]->AddToLevel(m_level, tilePos - m_cameraPos))
				{
					//Logger::Write("Tool placed at (%.2f, %.2f)", tilePos.x, tilePos.y);
				}
				else
				{
					//Logger::Write("Error, failed to place tool %i", m_activeTool);
				}
			}
		}
	}
}