#pragma once

#ifndef __PIRATEGAME_LEVEL_H__
#define __PIRATEGAME_LEVEL_H__

// Library Includes

// Local Includes
#include "../core/core.h"
#include "../core/utility/refcount.h"

using namespace Papyrus;

class ICharacter;
class CTile;
class CPlatform;

class CLevel : public TRefCount<CLevel>
{
	// Member Functions
public:
	CLevel();
	virtual ~CLevel();

	Bool			Initialise(Int8* _setup);
	Bool			ShutDown();

	void			Process(Float32 _delta);
	void			Render();

	// Member Variables
protected:
	Sprite::ISprite*	m_background;
	ICharacter*			m_playable;
	CTile**				m_tiles;
	CPlatform**			m_platforms;

	Int32				m_levelNumber;
	Int32				m_numTiles;
	Int32				m_numPlatforms;
};

#endif // __PIRATEGAME_LEVEL_H__