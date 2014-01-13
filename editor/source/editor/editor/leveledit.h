#pragma once

#ifndef __EDITOR_LEVELEDIT_H__
#define __EDITOR_LEVELEDIT_H__

// Library Includes

// Local Includes
#include "editor.h"

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

	virtual void		Notify(SDL_Event* _e);

	// Member Variables
protected:
	CLevel*				m_level;
};

#endif // __EDITOR_LEVELEDIT_H__