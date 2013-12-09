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
	MAX_TYPE
} ETileType;

class CTile : public TRefCount<CTile>
{
	// Member Functions
public:
	CTile() {}
	~CTile() {}

	Bool		Initialise(Int8* _spritesheet, VECTOR2 _pos, ETileType _type)
	{
		m_sprite = Sprite::CreateSprite(_spritesheet, 0, false);
		assert(m_sprite);
		m_sprite->AddRef();
		m_sprite->SetScale(50, 50);
		m_clips.w = 50;
		m_clips.h = 50;
		m_pos = _pos;

		switch(_type)
		{
		case TYPE_TOP_LEFT:
			m_clips.x = 0;
			m_clips.y = 0;
			break;
		case TYPE_TOP_MID:
			m_clips.x = 50;
			m_clips.y = 0;
			break;
		case TYPE_TOP_RIGHT:
			m_clips.x = 100;
			m_clips.y = 0;
			break;
		case TYPE_MID_LEFT:
			m_clips.x = 0;
			m_clips.y = 50;
			break;
		case TYPE_MID_MID:
			m_clips.x = 50;
			m_clips.y = 50;
			break;
		case TYPE_MID_RIGHT:
			m_clips.x = 100;
			m_clips.y = 50;
			break;
		case TYPE_BOT_LEFT:
			m_clips.x = 0;
			m_clips.y = 100;
			break;
		case TYPE_BOT_MID:
			m_clips.x = 50;
			m_clips.y = 100;
			break;
		case TYPE_BOT_RIGHT:
			m_clips.x = 100;
			m_clips.y = 100;
			break;
		default:
			break;
		}

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
	Sprite::ISprite*	m_sprite;
	SDL_Rect			m_clips;
	VECTOR2				m_pos;
};

#endif // __PIRATEGAME_LEVEL_TILE_H__