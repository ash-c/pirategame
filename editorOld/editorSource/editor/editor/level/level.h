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

	Bool			Save();

	void			CameraPos(VECTOR2 _pos);

	void			AddTile(VECTOR2 _pos);

	// Member Variables
protected:

	SDL_Surface*		m_surface;
	SDL_Texture*		m_grid;

	Sprite::ISprite*	m_background;
	ICharacter*			m_playable;
	CTile**				m_tiles;
	CPlatform**			m_platforms;

	VECTOR2				m_cameraPos;
	Int32				m_levelNumber;
	Int32				m_numTiles;
	Int32				m_numPlatforms;
	Int32				m_numRects;
};

#endif // __PIRATEGAME_LEVEL_H__