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
class CPlatform;
class CEnemy;

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

	// Member Variables
protected:
	Sprite::ISprite*	m_background;
	ICharacter*			m_playable;
	CTile**				m_tiles;
	CEnemy**			m_enemies;
	CPlatform**			m_platforms;

	VECTOR2				m_cameraPos;

	Int32				m_levelNumber;
	Int32				m_numTiles;
	Int32				m_numEnemies;
	Int32				m_numPlatforms;
	Int32				m_screenW;
	Int32				m_screenH;
};

#endif // __PIRATEGAME_LEVEL_H__