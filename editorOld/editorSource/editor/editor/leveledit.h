#pragma once

#ifndef __EDITOR_LEVELEDIT_H__
#define __EDITOR_LEVELEDIT_H__

// Library Includes

// Local Includes
#include "editor.h"
#include "tool.h"

class CLevel;

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

	// Member Variables
protected:
	CLevel*				m_level;
	ITool*				m_tools[MAX_TOOL];
	VECTOR2				cameraPos;
	EToolType			m_activeTool;
	Int32				m_screenHeight;
	Int32				m_screenWidth;
	Bool				m_rightMouseDown;
};

#endif // __EDITOR_LEVELEDIT_H__