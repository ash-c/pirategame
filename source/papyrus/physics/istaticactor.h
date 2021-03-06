#pragma once

#ifndef __PAPYRUS_PHYSICS_STATICACTOR_H__
#define __PAPYRUS_PHYSICS_STATICACTOR_H__

// Library Includes

// Local Includes
#include "iactor.h"
#include "../renderer/renderer.h"

namespace Papyrus
{
	namespace Physics
	{
		class IStaticActor : public IActor
		{
			// Member Functions
		public:
			IStaticActor() {}

			virtual ~IStaticActor() {}

			virtual Bool	Initialise(VECTOR2 _pos, VECTOR2 _scale) = 0;
			virtual Bool	ShutDown() = 0;

			virtual void	Process(Float32 _frameTime) {}

			virtual void	Interpolate(Float32 _alpha) {}

			virtual void	RenderDebug(VECTOR2 _camPos)
			{
				SDL_Rect rect = m_bounds.rect;
				rect.x += static_cast<Int32>(_camPos.x);
				rect.y -= static_cast<Int32>(_camPos.y);
				Renderer::activeRenderer->DrawRect(&rect, m_vCollision || m_hCollision);
			}

			virtual void	SetYPos(Float32 _y) {}
			virtual void	SetXPos(Float32 _x) {}

			virtual void	SetPosition(VECTOR2 _v) { m_pos = _v; } 
			virtual VECTOR2	GetPosition() { return m_pos; }

			virtual void	SetScale(VECTOR2 _v) { }

			virtual void	SetOwner(void* _owner) { }
			virtual void*	GetOwner() { return 0; }

			// Member Variables
		protected:
			VECTOR2			m_pos;
		};
	}
}

#endif // __PAPYRUS_PHYSICS_STATICACTOR_H__