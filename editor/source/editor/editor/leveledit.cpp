
// Library Includes

// Local Includes
#include "leveledit.h"
#include "level\level.h"

#include "tool.h"
#include "tools\tiletool.h"

CLevelEdit::CLevelEdit()
	: m_level(0)
	, m_activeTool(TOOL_TILE)
	, m_screenHeight(0)
	, m_screenWidth(0)
	, m_rightMouseDown(false)
{
}

CLevelEdit::~CLevelEdit()
{
}

Bool CLevelEdit::Initialise()
{
	CREATEPOINTER(m_level, CLevel);
	assert(m_level);
	VALIDATE(m_level->Initialise("data/levels/newlevel.json"));
	m_level->AddRef();

	m_screenHeight = Renderer::activeRenderer->GetHeight();
	m_screenWidth = Renderer::activeRenderer->GetWidth();

	SDL_memset(m_tools, 0, sizeof(ITool*) * MAX_TOOL);

	// Initialise tools.
	CREATEPOINTER(m_tools[TOOL_TILE], CToolTile);
	assert(m_tools[TOOL_TILE]);
	VALIDATE(m_tools[TOOL_TILE]->Initialise());
	
	// Currently used tool
	m_tools[m_activeTool]->AddRef();

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
	if (0 != m_level) m_level->CameraPos(cameraPos);
}

void CLevelEdit::Render()
{
	if (0 != m_level) m_level->Render();
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
			if (INVALID_TOOL != m_activeTool && MAX_TOOL > m_activeTool)
			{
				// Place tool.
				if (m_tools[m_activeTool]->AddToLevel(m_level))
				{
					Logger::Write("Tool placed.");
				}
				else
				{
					Logger::Write("Error, failed to place tool %i", m_activeTool);
				}
			}
		}
	}
	else if (SDL_MOUSEBUTTONUP == _e->type)
	{
		if (SDL_BUTTON_RIGHT == _e->button.button)
		{
			m_rightMouseDown = false;
		}
	}
	else if (SDL_MOUSEMOTION == _e->type)
	{
		if (m_rightMouseDown)
		{
			cameraPos.x += _e->motion.xrel;
			cameraPos.y += _e->motion.yrel;

			if (cameraPos.y < 0.0f) cameraPos.y = 0.0f;
			if (cameraPos.y > (LEVEL_HEIGHT - m_screenHeight)) cameraPos.y = static_cast<Float32>(LEVEL_HEIGHT - m_screenHeight);

			if (cameraPos.x > 0.0f) cameraPos.x = 0.0f; 
			if (cameraPos.x < -(LEVEL_WIDTH - m_screenWidth)) cameraPos.x = -static_cast<Float32>(LEVEL_WIDTH - m_screenWidth);
		}
	}
}