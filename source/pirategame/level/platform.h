#pragma once

#ifndef __PIRATEGAME_LEVEL_PLATFORM_H__
#define __PIRATEGAME_LEVEL_PLATFORM_H__

// Library Includes

// Local Includes
#include "tile.h"

class CPlatform : public ITile
{
	// Member Functions
public:
	CPlatform();
	virtual ~CPlatform();

	virtual Bool		Initialise();

	// Member Variables
protected:
};

#endif // __PIRATEGAME_LEVEL_PLATFORM_H__