#pragma once

#ifndef __EDITOR_TOOL_H__
#define __EDITOR_TOOL_H__

// Library Includes

// Local Includes
#include "../../core/utility/refcount.h"
#include "level/level.h"

typedef enum _EToolType
{
	INVALID_TOOL = INVALID_ID,
	TOOL_REMOVE,
	TOOL_TILE,
	TOOL_PLATFORM,
	TOOL_BASIC_ENEMY,
	TOOL_PLAYER_START,
	TOOL_WATER,
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

	virtual Bool		AddToLevel(CLevel* _level, VECTOR2 _pos) = 0;

	// Member Variables
protected:
};

#endif // __EDITOR_TOOL_H__