#pragma once

#ifndef __PAPYRUS_PHYSICS_DYNAMICACTOR_H__
#define __PAPYRUS_PHYSICS_DYNAMICACTOR_H__

// Library Includes

// Local Includes
#include "iactor.h"

namespace Papyrus
{
	namespace Physics
	{
		class IDynamicActor : public IActor
		{
			// Member Functions
		public:
			IDynamicActor() 
				: m_zero(0.5f)
				, m_mass(0.0f)
			{
			}

			virtual ~IDynamicActor() {}

			virtual Bool	Initialise(VECTOR2 _maxVel, VECTOR2 _pos, Float32 _mass) = 0;
			virtual Bool	ShutDown() = 0;

			virtual void	SetMass(Float32 _f) { m_mass = _f; }
			virtual Float32	GetMass() { return m_mass; }

			virtual void	SetVelocity(VECTOR2 _v) { m_currState.vel = _v; }
			virtual VECTOR2	GetVelocity() { return m_currState.vel; }

			virtual void	SetPosition(VECTOR2 _v) { m_currState.pos = _v; } 
			virtual VECTOR2	GetPosition() { return m_currState.pos; }

			virtual void	ApplyForce(VECTOR2 _force)
			{
				// f = ma, so a = f/m
				VECTOR2 accel = _force / m_mass;
				
				m_currState.vel += accel;
			}

			virtual void	Process(Float32 _delta)
			{
				if (m_active)
				{
					// apply friction
					if (m_currState.vel.x > 0.0f)
					{
						m_currState.vel.x -= 250.0f * _delta;
					}
					else  if (m_currState.vel.x < 0.0f)
					{
						m_currState.vel.x += 250.0f * _delta;
					}

					// change position
					if (m_currState.vel.x > m_maxVel.x)
					{
						m_currState.vel.x = m_maxVel.x;
					} 
					else if (m_currState.vel.x < -m_maxVel.x)
					{
						m_currState.vel.x = -m_maxVel.x;
					}

					m_currState.pos += m_currState.vel * _delta; 					

					// Check if at rest
					if (m_currState.vel.x < m_zero && m_currState.vel.x > -m_zero)
					{
						m_currState.acc.x = 0.0f;
						m_currState.vel.x = 0.0f;
						m_active = false;
					}
				}
			}

		protected:
			typedef struct _TState
			{
				VECTOR2		pos;	// position
				VECTOR2		vel;	// velocity
				VECTOR2		acc;	// acceleration
			} State;

			// Member Variables
		protected:
			State			m_currState;

			VECTOR2			m_maxVel;

			Float32			m_zero;
			Float32			m_mass;
		};
	}
}

#endif // __PAPYRUS_PHYSICS_DYNAMICACTOR_H__