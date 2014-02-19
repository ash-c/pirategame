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

	virtual void		SetTool(Int32 _new);

	virtual void		Notify(SDL_Event* _e);
	virtual void		Load(const Int8* _path);

	// Member Variables
protected:
};

#endif // __EDITOR_INTERFACEEDIT_H__