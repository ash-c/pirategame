#pragma once

#ifndef __PIRATEGAME_LEVEL_WATER_H__
#define __PIRATEGAME_LEVEL_WATER_H__

// Library Includes

// Local Includes
#include "tile.h"

class CWater : public CTile
{
	// Member Functions
public:
	CWater();
	virtual ~CWater();

	virtual Bool		Initialise(Int8* _spritesheet, VECTOR2 _pos, ETileType _type);

	// Member Variables
protected:
};

#endif // __PIRATEGAME_LEVEL_WATER_H__