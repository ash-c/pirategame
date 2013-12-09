#pragma once

#ifndef __PIRATEGAME_LEVEL_PLATFORM_H__
#define __PIRATEGAME_LEVEL_PLATFORM_H__

// Library Includes

// Local Includes
#include "tile.h"
#include "../core/core.h"

using namespace Papyrus;

class CPlatform
{
	// Member Functions
public:
	CPlatform();
	virtual ~CPlatform();

	virtual Bool		Initialise(Int8* _spritesheet);

	// Member Variables
protected:
	CTile*				m_tiles;
};

#endif // __PIRATEGAME_LEVEL_PLATFORM_H__