#pragma once

#ifndef __EDITOR_H__
#define __EDITOR_H__

// Library Includes
#include <lua.h>

// Local Includes
#include "../../core/utility/singleton.h"
#include "../../core/utility/refcount.h"
#include "../../input/input.h"
#include "../../input/observer.h"
#include "../../logging/logger.h"
#include "../../ui/ui.h"

class IEditor : public TRefCount<IEditor>, Papyrus::Input::IInputObserver, public TSingleton<IEditor>
{
	friend class TSingleton<IEditor>;

	// Member Functions
public:
	IEditor() 
	: m_escMenu(0)
	{}
	virtual ~IEditor() {}

	virtual Bool		Initialise()
	{
		Papyrus::Input::inputManager->Register(this);
		return true;
	}
	virtual Bool		ShutDown() = 0;

	virtual void		Process(Float32 _delta) = 0;
	virtual void		Render() = 0;

	virtual Bool		Save(Int8* _path = 0) = 0;

	virtual void		Notify(SDL_Event* _e) = 0;

	virtual void		SetTool(Int32 _new) = 0;
	virtual void		Load(const Int8* _path) = 0;

	virtual void		ToggleEscMenu() = 0;

	static Int32		ChangeTool(lua_State* L)
	{
		Int32 i = static_cast<Int32>(lua_tonumber(L, 1));

		sm_pTheInstance->SetTool(i);

		Papyrus::Logger::Write("Tool changed to %i", i);

		return 0;
	}

	static Int32		LoadFile(lua_State* L)
	{
		const Int8* path = luaL_checkstring(L, 1);		

		sm_pTheInstance->Load(path);

		return 0;
	}

	// Member Variables
protected:
	Papyrus::UI::IUIInterface*	m_escMenu;
};

#endif // __EDITOR_H__