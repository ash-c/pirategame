#pragma once

#ifndef __EDITOR_INTERFACEEDIT_H__
#define __EDITOR_INTERFACEEDIT_H__

// Library Includes

// Local Includes
#include "editor.h"

class CIntEdit : public IEditor
{
	// Member Functions
public:
	CIntEdit();
	virtual ~CIntEdit();

	virtual Bool		Initialise();
	virtual Bool		ShutDown();

	virtual void		Process(Float32 _delta);
	virtual void		Render();

	virtual Bool		Save();

	virtual void		Notify(SDL_Event* _e);

	// Member Variables
protected:
};

#endif // __EDITOR_INTERFACEEDIT_H__