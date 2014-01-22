#pragma once

#ifndef __PIRATEGAME_LEVEL_TILE_H__
#define __PIRATEGAME_LEVEL_TILE_H__

// Library Includes

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
static const Int32 TILE_WIDTH_DOUBLE = TILE_WIDTH * 2;
static const Int32 TILE_HEIGHT_DOUBLE = TILE_HEIGHT * 2;
static const Int32 TILE_HEIGHT_TRIPLE = TILE_HEIGHT * 3;
static const Int32 TILE_HEIGHT_QUAD = TILE_HEIGHT * 4;

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
	ETileType	GetType();

	// Member Variables
protected:
	Physics::IActor*	m_actor;
	Sprite::ISprite*	m_sprite;
	SDL_Rect			m_clips;
	VECTOR2				m_pos;
	Int32				m_screenW;
	Int32				m_screenH;
	ETileType			m_type;
};

#endif // __PIRATEGAME_LEVEL_TILE_H__