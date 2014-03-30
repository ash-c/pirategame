#pragma once

#ifndef __PIRATEGAME_LEVEL_H__
#define __PIRATEGAME_LEVEL_H__

// Library Includes

// Local Includes
#include "../../core/core.h"
#include "../../core/utility/refcount.h"

using namespace Papyrus;

class ICharacter;
class CTile;
class CWater;
class CPlatform;
class CEnemy;
class CParallax;

static const Int32			LEVEL_WIDTH = 10000;
static const Int32			LEVEL_HEIGHT = 2000;

class CLevel : public TRefCount<CLevel>
{
	// Member Functions
public:
	CLevel();
	virtual ~CLevel();

	Bool			Initialise(Int8* _setup);
	Bool			ShutDown();

	Bool			Reset();

	void			Process(Float32 _delta);
	void			Render();

	Bool			IsComplete() { return m_complete; }

	// Member Variables
protected:
	Sprite::ISprite*	m_background;
	ICharacter*			m_playable;
	CTile**				m_tiles;
	CWater**			m_water;
	CEnemy**			m_enemies;
	CPlatform**			m_platforms;
	CParallax**			m_parallax;

	VECTOR2				m_cameraPos;

	Int32				m_levelNumber;
	Int32				m_numTiles;
	Int32				m_numWater;
	Int32				m_numEnemies;
	Int32				m_numPlatforms;
	Int32				m_paraCount;
	Int32				m_screenW;
	Int32				m_screenH;

	Bool				m_complete;
};

#endif // __PIRATEGAME_LEVEL_H__