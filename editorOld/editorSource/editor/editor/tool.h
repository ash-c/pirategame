#pragma once

#ifndef __EDITOR_TOOL_H__
#define __EDITOR_TOOL_H__

// Library Includes

// Local Includes
#include "../core/utility/refcount.h"
#include "level\level.h"

typedef enum _EToolType
{
	INVALID_TOOL = INVALID_ID,
	TOOL_TILE,
	TOOL_PLATFORM,
	TOOL_ENEMY,
	TOOL_PLAY_START,
	MAX_TOOL
} EToolType;

class ITool : public TRefCount<ITool>
{
	// Member Functions
public:
	ITool() {}
	virtual ~ITool() {}

	virtual Bool		Initialise() = 0;
	virtual Bool		ShutDown() = 0;

	virtual Bool		AddToLevel(CLevel* _level) = 0;

	// Member Variables
protected:
};

#endif // __EDITOR_TOOL_H__