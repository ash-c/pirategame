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
	CTile() 
		: m_actor(0)
		, m_sprite(0)
	{
		SDL_memset(&m_clips, 0, sizeof(Int32) * 4);
	}

	~CTile() {}

	Bool		Initialise(Int8* _spritesheet, VECTOR2 _pos, ETileType _type)
	{
		m_sprite = Sprite::CreateSprite(_spritesheet, 0, false);
		assert(m_sprite);
		m_sprite->AddRef();
		m_sprite->SetScale(TILE_WIDTH, TILE_HEIGHT);
		m_clips.w = TILE_WIDTH;
		m_clips.h = TILE_HEIGHT;
		m_pos = _pos;

		switch(_type)
		{
		case TYPE_TOP_LEFT:
			m_clips.x = 0;
			m_clips.y = 0;
			break;
		case TYPE_TOP_MID:
			m_clips.x = TILE_WIDTH;
			m_clips.y = 0;
			break;
		case TYPE_TOP_RIGHT:
			m_clips.x = TILE_WIDTH_DOUBLE;
			m_clips.y = 0;
			break;
		case TYPE_MID_LEFT:
			m_clips.x = 0;
			m_clips.y = TILE_HEIGHT;
			break;
		case TYPE_MID_MID:
			m_clips.x = TILE_WIDTH;
			m_clips.y = TILE_HEIGHT;
			break;
		case TYPE_MID_RIGHT:
			m_clips.x = TILE_WIDTH_DOUBLE;
			m_clips.y = TILE_HEIGHT;
			break;
		case TYPE_BOT_LEFT:
			m_clips.x = 0;
			m_clips.y = TILE_HEIGHT_DOUBLE;
			break;
		case TYPE_BOT_MID:
			m_clips.x = TILE_WIDTH;
			m_clips.y = TILE_HEIGHT_DOUBLE;
			break;
		case TYPE_BOT_RIGHT:
			m_clips.x = TILE_WIDTH_DOUBLE;
			m_clips.y = TILE_HEIGHT_DOUBLE;
			break;
		default:
			break;
		}

<<<<<<< HEAD:editorOld/editorSource/editor/editor/level/tile.h
	VECTOR2		GetPos();
	ETileType	GetType();
=======
		m_actor = Physics::CreateStaticActor(m_pos, VECTOR2(static_cast<Float32>(TILE_WIDTH),static_cast<Float32>(TILE_HEIGHT)));
		assert(m_actor);
>>>>>>> parent of e8b7430... Tile placement working in level editor.:editor/source/editor/editor/level/tile.h

		return true;
	}

	Bool		ShutDown()
	{
		PY_SAFE_RELEASE(m_sprite);
		return true;
	}

	void		Render()
	{
		m_sprite->SetClip(&m_clips);
		m_sprite->SetPosition(static_cast<Int32>(m_pos.x), static_cast<Int32>(m_pos.y));
		m_sprite->Render();
	}

	// Member Variables
protected:
	Physics::IActor*	m_actor;
	Sprite::ISprite*	m_sprite;
	SDL_Rect			m_clips;
	VECTOR2				m_pos;
<<<<<<< HEAD:editorOld/editorSource/editor/editor/level/tile.h
	VECTOR2				m_cameraPos;
	Int32				m_screenW;
	Int32				m_screenH;
	ETileType			m_type;
=======
>>>>>>> parent of e8b7430... Tile placement working in level editor.:editor/source/editor/editor/level/tile.h
};

#endif // __PIRATEGAME_LEVEL_TILE_H__