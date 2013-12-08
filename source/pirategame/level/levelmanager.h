#pragma once

#ifndef __PIRATEGAME_LEVEL_MANAGER_H__
#define __PIRATEGAME_LEVEL_MANAGER_H__

// Library Includes

// Local Includes
#include "../core/core.h"
#include "../core/utility/singleton.h"

class CLevel;

class CLevelManager : public TSingleton<CLevelManager>
{
	friend class TSingleton<CLevelManager>;

	// Member Functions
public:
	virtual ~CLevelManager();

	Bool			Initialise();
	Bool			ShutDown();

	void			Process(Float32 _delta);
	void			Render();

	Bool			LoadLevel(Int32 _lvl);

protected:
	CLevelManager();

	// Member Variables
protected:	
	CLevel*			m_currLevel;
	Int32			m_numlevels;
};

#endif // __PIRATEGAME_LEVEL_MANAGER_H__