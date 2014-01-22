#pragma once

#ifndef __PAPYRUS_PHYSICS_ACTOR_H__
#define __PAPYRUS_PHYSICS_ACTOR_H__

// Library Includes
#include <SDL.h>

// Local Includes
#include "../core/utility/refcount.h"

namespace Papyrus
{
	namespace Physics
	{
		typedef enum _EType
		{
			INVALID_TYPE = INVALID_ID,
			TYPE_STATIC,
			TYPE_PLAYER,
			TYPE_PLATFORM,
			MAX_TYPE
		} EType;

		class IActor : public TRefCount<IActor>
		{
			// Member Functions
		public:
			IActor() 
				: m_player(0)
				, m_type(INVALID_TYPE)
				, m_active(false)
				, m_collided(false)
				, m_ppCollision(false)
			{}

			virtual ~IActor() {}

			virtual Bool	ShutDown() = 0;

			virtual void	Process(Float32 _frameTime) = 0;
			virtual void	RenderDebug(VECTOR2 _camPos) = 0;

			virtual void	SetPosition(VECTOR2 _v) = 0;
			virtual VECTOR2	GetPosition() = 0;

			virtual SDL_Rect GetRect() { return m_bounds.rect; }

			virtual Bool	IsActive() { return m_active; }
			virtual void	SetActive(Bool _b) { m_active = _b; } 

			virtual Bool	IsCollided() { return m_collided; }
			virtual void	SetCollided(Bool _b) { m_collided = _b; }

			virtual Bool	IsPPCollided() { return m_ppCollision; }
			virtual void	SetPPCollided(IActor* _player, Bool _b) { m_ppCollision = _b; m_player = _player; }

			virtual void	UpdatePlayer(VECTOR2 _pos) 
			{ 
				if (0 != m_player)
				{
					VECTOR2 pos = m_player->GetPosition();
					pos.x += _pos.x;
					m_player->SetPosition(pos);
				}
			}

			virtual EType	GetType() { return m_type; }

			// Member Variables
		protected:
			typedef struct _TAABB
			{
				SDL_Rect	rect;

				Float32		topLX;
				Float32		topLY;
				Float32		botRX;
				Float32		botRY;
			} AABB;

			IActor*			m_player;
			AABB			m_bounds;
			EType			m_type;
			Bool			m_active;
			Bool			m_collided;
			Bool			m_ppCollision; // player on platform
		};
	}
}

#endif // __PAPYRUS_PHYSICS_ACTOR_H__