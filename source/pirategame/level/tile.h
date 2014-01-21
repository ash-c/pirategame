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

class CTile : public TRefCount<CTile>
{
	// Member Functions
public:
	CTile() 
		: m_actor(0)
		, m_sprite(0)
		, m_screenW(0)
		, m_screenH(0)
	{
		SDL_memset(&m_clips, 0, sizeof(Int32) * 4);
	}

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
		case TYPE_LEFT:
			m_clips.x = 0;
			m_clips.y = 150;
			break;
		case TYPE_MID:
			m_clips.x = 50;
			m_clips.y = 150;
			break;
		case TYPE_RIGHT:
			m_clips.x = 100;
			m_clips.y = 150;
		default:
			break;
		}

		m_actor = Physics::CreateStaticActor(m_pos, VECTOR2(50.0f,50.0f));
		assert(m_actor);
	
		m_screenW = Renderer::activeRenderer->GetWidth();
		m_screenH = Renderer::activeRenderer->GetHeight();

		return true;
	}

	Bool		ShutDown()
	{
		PY_SAFE_RELEASE(m_sprite);
		return true;
	}

	void		Render(VECTOR2 _camPos)
	{
		if ((m_pos.x + _camPos.x) > 0.0f && (m_pos.y + _camPos.y) > 0.0f &&
			(m_pos.x + _camPos.x) < m_screenW && (m_pos.y + _camPos.y) < m_screenH)
		{
			m_actor->SetActive(true);
			m_sprite->SetClip(&m_clips);
			m_sprite->SetPosition(static_cast<Int32>(m_pos.x + _camPos.x), static_cast<Int32>(m_pos.y + _camPos.y));
			m_sprite->Render();
		}
		else if (m_actor->IsActive())
		{
			m_actor->SetActive(false);
		}
	}

	// Member Variables
protected:
	Physics::IActor*	m_actor;
	Sprite::ISprite*	m_sprite;
	SDL_Rect			m_clips;
	VECTOR2				m_pos;
	Int32				m_screenW;
	Int32				m_screenH;
};

#endif // __PIRATEGAME_LEVEL_TILE_H__