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
			{}

			virtual ~IActor() {}

			virtual Bool	Initialise(VECTOR2 _maxVel, VECTOR2 _maxAcc, VECTOR2 _pos, VECTOR2 _scale, Float32 _mass) = 0;
			virtual Bool	ShutDown() = 0;

			virtual void	Process(Float32 _frameTime) = 0;
			virtual void	ProcessInterpolate(Float32 _alpha) = 0;
			virtual void	RenderDebug() = 0;

			virtual VECTOR2	GetPosition() = 0;

			virtual Bool	IsActive() { return m_active; }
			virtual void	SetActive(Bool _b) { m_active = _b; } 

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
		};
	}
}

#endif // __PAPYRUS_PHYSICS_ACTOR_H__