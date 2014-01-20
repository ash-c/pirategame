#pragma once

#ifndef __PIRATEGAME_LEVEL_TILE_H__
#define __PIRATEGAME_LEVEL_TILE_H__

// Library Includes
#include <vector>

// Local Includes
#include "../core/core.h"
#include "../core/utility/refcount.h"

using namespace Papyrus;

typedef enum _ETileType
{
	INVALID_TYPE = INVALID_ID,
	TYPE_TOP_LEFT,
	TYPE_TOP_MID,
	TYPE_TOP_RIGHT,
	TYPE_MID_LEFT,
	TYPE_MID_MID,
	TYPE_MID_RIGHT,
	TYPE_BOT_LEFT,
	TYPE_BOT_MID,
	TYPE_BOT_RIGHT,
	TYPE_LEFT,
	TYPE_MID,
	TYPE_RIGHT,
	TYPE_TOP_VERT,
	TYPE_MID_VERT,
	TYPE_BOT_VERT,
	TYPE_ALONE,
	MAX_TYPE
} ETileType;

static const Int32 TILE_WIDTH = 50;
static const Int32 TILE_HEIGHT = 50;
static const Int32 TILE_WIDTH_DOUBLE = 100;
static const Int32 TILE_HEIGHT_DOUBLE = 100;

class CTile : public TRefCount<CTile>
{
	// Member Functions
public:
	CTile();
	~CTile();

	Bool		Initialise(Int8* _spritesheet, VECTOR2 _pos, ETileType _type);
	Bool		ShutDown();

	void		Render(VECTOR2 _camPos);

	void		SetType(ETileType _type);

	VECTOR2		GetPos();

	void		AddLinked(CTile* _link);
	void		UpdateType();

	// Member Variables
protected:
	std::vector<CTile*>	m_linked;
	VECTOR2				m_around[8];

	Physics::IActor*	m_actor;
	Sprite::ISprite*	m_sprite;
	SDL_Rect			m_clips;
	VECTOR2				m_pos;
	VECTOR2				m_cameraPos;
};

#endif // __PIRATEGAME_LEVEL_TILE_H__