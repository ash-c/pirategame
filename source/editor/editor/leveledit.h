#pragma once

#ifndef __EDITOR_LEVELEDIT_H__
#define __EDITOR_LEVELEDIT_H__

// Library Includes

// Local Includes
#include "editor.h"
#include "tool.h"

class CLevel;
class Papyrus::UI::IUIInterface;

class CLevelEdit : public IEditor
{
	// Member Functions
public:
	CLevelEdit();
	virtual ~CLevelEdit();

	virtual Bool		Initialise();
	virtual Bool		ShutDown();

	virtual void		Process(Float32 _delta);
	virtual void		Render();

	virtual Bool		Save();

	virtual void		Notify(SDL_Event* _e);

	virtual void		SetTool(Int32 _new);
	virtual void		Load(const Int8* _path);

	virtual void		ToggleEscMenu();

	// Member Variables
protected:
	CLevel*				m_level;
	ITool*				m_tools[MAX_TOOL];
	Papyrus::UI::IUIInterface* m_toolContext;

	VECTOR2				m_cameraPos;
	EToolType			m_activeTool;

	Float32				m_tileDelay;

	Int32				m_screenHeight;
	Int32				m_screenWidth;

	Bool				m_rightMouseDown;
	Bool				m_leftMouseDown;
	Bool				m_mouseMoved;
};

#endif // __EDITOR_LEVELEDIT_H__