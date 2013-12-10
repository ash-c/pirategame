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
		class IActor : public TRefCount<IActor>
		{
			// Member Functions
		public:
			IActor() 
				: m_active(false)
				, m_collided(false)
			{}

			virtual ~IActor() {}

			virtual Bool	ShutDown() = 0;

			virtual void	Process(Float32 _frameTime) = 0;
			virtual void	RenderDebug() = 0;

			virtual void	SetPosition(VECTOR2 _v) = 0;
			virtual VECTOR2	GetPosition() = 0;

			virtual SDL_Rect GetRect() { return m_bounds.rect; }

			virtual Bool	IsActive() { return m_active; }
			virtual void	SetActive(Bool _b) { m_active = _b; } 
			virtual void	SetCollided(Bool _b) { m_collided = _b; }

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

			AABB			m_bounds;
			Bool			m_active;
			Bool			m_collided;
		};
	}
}

#endif // __PAPYRUS_PHYSICS_ACTOR_H__