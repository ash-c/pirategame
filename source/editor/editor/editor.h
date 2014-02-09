#pragma once

#ifndef __EDITOR_H__
#define __EDITOR_H__

// Library Includes

// Local Includes
#include "../../core/utility/refcount.h"
#include "../../input/input.h"
#include "../../input/observer.h"

class IEditor : public TRefCount<IEditor>, Papyrus::Input::IInputObserver
{
	// Member Functions
public:
	IEditor() {}
	virtual ~IEditor() {}

	virtual Bool		Initialise()
	{
		Papyrus::Input::inputManager->Register(this);
		return true;
	}
	virtual Bool		ShutDown() = 0;

	virtual void		Process(Float32 _delta) = 0;
	virtual void		Render() = 0;

	virtual Bool		Save() = 0;

	virtual void		Notify(SDL_Event* _e) = 0;

	// Member Variables
protected:
};

#endif // __EDITOR_H__