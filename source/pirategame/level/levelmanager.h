#pragma once

#ifndef __PIRATEGAME_LEVEL_MANAGER_H__
#define __PIRATEGAME_LEVEL_MANAGER_H__

// Library Includes

// Local Includes
#include "../../core/core.h"
#include "../../core/utility/singleton.h"

class CLevel;

class CLevelManager : public TSingleton<CLevelManager>
{
	friend class TSingleton<CLevelManager>;

	// Member Functions
public:
	virtual ~CLevelManager();

	Bool			Initialise(Int32 _numLevels, Int32 _currLevel);
	Bool			ShutDown();

	void			Process(Float32 _delta);
	void			Render();

	Bool			LoadLevel(Int8* _lvl);

	Bool			ResetLevel();
	Bool			IsFinished() { return m_finLevels; }

	static Int32	RestartLevel(lua_State* L);

protected:
	CLevelManager();

	// Member Variables
protected:	
	CLevel*			m_current;
	Int32			m_numlevels;
	Int32			m_currLevel;

	Bool			m_finLevels;
};

#endif // __PIRATEGAME_LEVEL_MANAGER_H__