#pragma once

#ifndef __PIRATEGAME_LEVEL_H__
#define __PIRATEGAME_LEVEL_H__

// Library Includes
#include <vector>

// Local Includes
#include "../../core/core.h"
#include "../../core/utility/refcount.h"

using namespace Papyrus;

class ICharacter;
class CTile;
class CWater;
class CPlatform;

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

	void			Process(Float32 _delta);
	void			Render();

	Bool			Save(Int8* _path = 0);

	void			CameraPos(VECTOR2 _pos);

	Bool			AddTile(VECTOR2 _pos);
	Bool			RemoveTile(VECTOR2 _pos);

	Bool			AddWater(VECTOR2 _pos);
	Bool			RemoveWater(VECTOR2 _pos);

	Bool			AddEnemy(VECTOR2 _pos, Physics::EType _type);
	Bool			RemoveEnemy(VECTOR2 _pos);

	Bool			SetPlayerStart(VECTOR2 _pos);

	Bool			AddMovingPlatform(VECTOR2 _pos);
	Bool			RemoveMovingPlatform(VECTOR2 _pos);

private:
	void			CheckAgainstGrid(VECTOR2* _pos);
	Bool			CheckForExistingTile(VECTOR2* _pos);

	// Member Variables
protected:
	SDL_Surface*		m_surface;
	SDL_Texture*		m_grid;

	Sprite::ISprite*	m_background;
	ICharacter*			m_playable;
	std::vector<ICharacter*>	m_enemies;
	std::vector<CTile*>	m_tiles;
	std::vector<CWater*> m_water;
	std::vector<CPlatform*>	m_platforms;

	SDL_Rect*			m_gridRects;

	VECTOR2				m_cameraPos;
	Int32				m_levelNumber;
	Int32				m_numTiles;
	Int32				m_numWater;
	Int32				m_numQuicksand;
	Int32				m_numPlatforms;
	Int32				m_numEnemies;
	Int32				m_numRects;

	Int8*				m_tileset;
};

#endif // __PIRATEGAME_LEVEL_H__