#pragma once

#ifndef __PIRATEGAME_LEVEL_TILE_H__
#define __PIRATEGAME_LEVEL_TILE_H__

// Library Includes

// Local Includes
#include "../core/core.h"
#include "../core/utility/refcount.h"

typedef enum _ETileType
{
	INVALID_TYPE = INVALID_ID,
	TYPE_TOP_LEFT,
	TYPE_TOP_MID,
	TYPE_TOP_RIGHT,
	TYPE_MID_LEFT,
	TYPE_MID_MID,
	TYPE_MID_RIGHT,
	TYPE_BOT_LEFT,
	TYPE_BOT_MID,
	TYPE_BOT_RIGHT,
	MAX_TYPE
} ETileType;

class ITile : public TRefCount<ITile>
{
	// Member Functions
public:
	ITile() {}
	~ITile() {}

	virtual Bool		Initialise() = 0;

	// Member Variables
protected:
};

#endif // __PIRATEGAME_LEVEL_TILE_H__