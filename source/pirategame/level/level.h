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
class CCoin;
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

	Bool			Initialise(Int8* _setup, Int32 _num);
	Bool			ShutDown();

	Bool			Reset();

	void			Process(Float32 _delta);
	void			Render();

	Bool			IsComplete() { return m_complete; }

	Bool			IsNext(VECTOR2 _pos);

	void			AddToScore(Int32 _amt) { m_score += _amt; }
	Int32			GetScore() { return m_score; }

	// Member Variables
protected:
	Sprite::ISprite*	m_background;
	Sprite::ISprite*	m_numbers;
	ICharacter*			m_playable;
	CTile**				m_tiles;
	CWater**			m_water;
	CCoin**				m_coins;
	CEnemy**			m_enemies;
	CPlatform**			m_platforms;
	CParallax**			m_parallax;

	SDL_Rect			m_numberClips[10];

	VECTOR2				m_numberPos;
	VECTOR2				m_numberScale;
	VECTOR2				m_cameraPos;

	Int32				m_numTiles;
	Int32				m_numWater;
	Int32				m_numEnemies;
	Int32				m_numPlatforms;
	Int32				m_numCoins;
	Int32				m_paraCount;
	Int32				m_screenW;
	Int32				m_screenH;
	Int32				m_score;
	Int32				m_levelNum;

	Bool				m_complete;
};

#endif // __PIRATEGAME_LEVEL_H__