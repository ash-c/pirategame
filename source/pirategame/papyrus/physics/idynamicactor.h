#pragma once

#ifndef __PAPYRUS_PHYSICS_DYNAMICACTOR_H__
#define __PAPYRUS_PHYSICS_DYNAMICACTOR_H__

// Library Includes

// Local Includes
#include "iactor.h"
#include "../renderer/renderer.h"

namespace Papyrus
{
	namespace Physics
	{
		class IDynamicActor : public IActor
		{
			// Member Functions
		public:
			IDynamicActor() 
				: m_zero(0.01f)
				, m_mass(0.0f)
			{
			}

			virtual ~IDynamicActor() {}

			virtual Bool	Initialise(VECTOR2 _maxVel, VECTOR2 _maxAcc, VECTOR2 _pos, VECTOR2 _scale, Float32 _mass) = 0;
			virtual Bool	ShutDown() = 0;

			virtual void	SetMass(Float32 _f) { m_mass = _f; }
			virtual Float32	GetMass() { return m_mass; }

			virtual void	SetVelocity(VECTOR2 _v) { m_currState.vel = _v; m_currState.preV = _v; }
			virtual VECTOR2	GetVelocity() { return m_currState.vel; }

			virtual void	SetPosition(VECTOR2 _v) { m_currState.pos = _v; m_currState.preP = _v; } 
			virtual VECTOR2	GetPosition() { return m_currState.pos; }

			virtual void	ApplyForce(VECTOR2 _force)
			{
				// f = ma, so a = f/m
				m_currState.acc += _force / m_mass;

				if (m_currState.acc.x > m_maxState.acc.x)
					{
						m_currState.acc.x = m_maxState.acc.x;
					} 
					else if (m_currState.acc.x < -m_maxState.acc.x)
					{
						m_currState.acc.x = -m_maxState.acc.x;
					}
			}

			virtual void	Process(Float32 _delta)
			{
				if (m_active)
				{
					// accelerate
					if ((m_currState.vel.x > 0 && m_currState.preV.x < 0) || (m_currState.vel.x < 0 && m_currState.preV.x > 0))
					{
						m_currState.acc.x = 0.0f;
						m_currState.vel.x = 0.0f;
						m_active = false;
					}

					m_currState.preV = m_currState.vel;
					m_currState.vel += m_currState.acc * _delta;

					// cap velocity
					if (m_currState.vel.x > m_maxState.vel.x)
					{
						m_currState.vel.x = m_maxState.vel.x;
					} 
					else if (m_currState.vel.x < -m_maxState.vel.x)
					{
						m_currState.vel.x = -m_maxState.vel.x;
					}

					// change position				
					m_currState.preP = m_currState.pos;
					m_currState.pos += m_currState.vel * _delta; 
					
					// Check if at rest
					if (m_currState.vel.x < m_zero && m_currState.vel.x > -m_zero)
					{
						m_currState.acc.x = 0.0f;
						m_currState.vel.x = 0.0f;
						m_active = false;
					}

					// Update bounds
					m_bounds.topLX = m_currState.pos.x - m_bounds.rect.w * 0.5f;
					m_bounds.topLY = m_currState.pos.y - m_bounds.rect.h * 0.5f;
					m_bounds.botRX = m_currState.pos.x + m_bounds.rect.w * 0.5f;
					m_bounds.botRY = m_currState.pos.y + m_bounds.rect.h * 0.5f;
					m_bounds.rect.x = static_cast<Int32>(m_bounds.topLX);
					m_bounds.rect.y = static_cast<Int32>(m_bounds.topLY);
				}
			}

			virtual void	ProcessInterpolate(Float32 _alpha)
			{
				m_currState.pos = m_currState.pos * _alpha + m_currState.preP * (1.0f - _alpha);
			}

			virtual void	RenderDebug()
			{
				Renderer::activeRenderer->DrawRect(&m_bounds.rect);
			}

		protected:
			typedef struct _TState
			{
				VECTOR2		pos;	// position
				VECTOR2		vel;	// velocity
				VECTOR2		acc;	// acceleration
				VECTOR2		preV;	// previous velocity
				VECTOR2		preP;	// previous position
			} State;

			// Member Variables
		protected:
			State			m_currState;
			State			m_maxState;

			Float32			m_zero;
			Float32			m_mass;
		};
	}
}

#endif // __PAPYRUS_PHYSICS_DYNAMICACTOR_H__