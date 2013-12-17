#pragma once

#ifndef __PIRATEGAME_LEVEL_PLATFORM_H__
#define __PIRATEGAME_LEVEL_PLATFORM_H__

// Library Includes

// Local Includes
#include "../core/core.h"

using namespace Papyrus;

class CTile;

class CPlatform
{
	// Member Functions
public:
	CPlatform();
	~CPlatform();

	Bool		Initialise(FileParser::IParser* _setup, Int32 _number, Int32 _platNum);
	Bool		ShutDown();

	void		Render();

	// Member Variables
protected:
	CTile**		m_tiles;
	Int32		m_numTiles;
};

#endif // __PIRATEGAME_LEVEL_PLATFORM_H__