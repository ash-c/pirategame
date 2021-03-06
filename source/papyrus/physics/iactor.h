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
			TYPE_BASIC_ENEMY,
			TYPE_WATER,
			TYPE_COIN,
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
				, m_hCollision(false)
				, m_vCollision(false)
				, m_ppCollision(false)
				, m_peCollision(false)
				, m_pwCollision(false)
				, m_pcCollision(false)
			{}

			virtual ~IActor() {}

			virtual Bool	ShutDown() = 0;

			virtual void	Process(Float32 _frameTime) = 0;
			virtual void	Interpolate(Float32 _alpha) = 0;
			virtual void	RenderDebug(VECTOR2 _camPos) = 0;

			virtual void	SetYPos(Float32 _y) = 0;
			virtual void	SetXPos(Float32 _x) = 0;

			virtual void	SetPosition(VECTOR2 _v) = 0;
			virtual VECTOR2	GetPosition() = 0;

			virtual void	SetScale(VECTOR2 _v) = 0;

			virtual void	SetOwner(void* _owner) = 0;
			virtual void*	GetOwner() = 0;

			virtual SDL_Rect GetRect() { return m_bounds.rect; }

			virtual Bool	IsActive() { return m_active; }
			virtual void	SetActive(Bool _b) { m_active = _b; } 

			virtual Bool	IsHCollided() { return m_hCollision; }
			virtual void	SetHCollided(Bool _b) { m_hCollision = _b; }

			virtual Bool	IsVCollided() { return m_vCollision; }
			virtual void	SetVCollided(Bool _b) { m_vCollision = _b; }

			virtual Bool	IsPPCollided() { return m_ppCollision; }
			virtual void	SetPPCollided(IActor* _player, Bool _b) { m_ppCollision = _b; m_player = _player; }

			virtual Bool	IsPECollided() { return m_peCollision; }
			virtual void	SetPECollided(Bool _b) { m_peCollision = _b; }

			virtual Bool	IsPWCollided() { return m_pwCollision; }
			virtual void	SetPWCollided(Bool _b) { m_pwCollision = _b; }

			virtual Bool	IsPCCollided() { return m_pcCollision; }
			virtual void	SetPCCollided(Bool _b) { m_pcCollision = _b; }

			virtual void	SetType(EType _e) { m_type = _e; }

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
			Bool			m_hCollision;
			Bool			m_vCollision;
			Bool			m_ppCollision; // player on platform
			Bool			m_peCollision; // player/enemy collision
			Bool			m_pwCollision; // player/water collision
			Bool			m_pcCollision; // player/coin collision
		};
	}
}

#endif // __PAPYRUS_PHYSICS_ACTOR_H__