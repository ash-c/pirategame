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
				: m_zero(10.0f)
				, m_mass(0.0f)
				, m_stationary(true)
			{
				m_currState.acc.y = 500.0f; // Gravity
			}

			virtual ~IDynamicActor() {}

			virtual Bool	Initialise(VECTOR2 _maxVel, VECTOR2 _maxAcc, VECTOR2 _pos, VECTOR2 _scale, Float32 _mass, EType _type) = 0;
			virtual Bool	ShutDown() = 0;

			virtual void	SetMass(Float32 _f) { m_mass = _f; }
			virtual Float32	GetMass() { return m_mass; }

			virtual void	SetVelocity(VECTOR2 _v) { m_currState.vel = _v; m_currState.preV = _v; }
			virtual VECTOR2	GetVelocity() { return m_currState.vel; }

			virtual void	SetPosition(VECTOR2 _v) 
			{ 
				m_currState.pos = _v; 
				m_currState.preP = _v; 
				UpdateBounds(); 
			} 

			virtual VECTOR2	GetPosition() 
			{ 
				return m_currState.pos; 
			}

			virtual Bool IsStationary() { return m_stationary; }

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

				if (m_currState.acc.y < -m_maxState.acc.y)
				{
					m_currState.acc.y = -m_maxState.acc.y;
				}
				m_stationary = false;
			}

			virtual void	Process(Float32 _delta)
			{
				if (m_ppCollision && m_type == Physics::EType::TYPE_PLAYER)
				{
					m_currState.acc.x = 0.0f;
					if ((m_currState.vel.x >= 250) || (m_currState.vel.x <= -250))
					{
						m_currState.vel.x = 0.0f;
						//m_active = false;
					}
				}

				m_currState.preV = m_currState.vel;
				if (m_type != Physics::EType::TYPE_PLATFORM)
				{
					m_currState.vel += m_currState.acc * _delta;
				}
					
				// cap velocity
				if (m_currState.vel.x > m_maxState.vel.x)
				{
					m_currState.vel.x = m_maxState.vel.x;
				} 
				else if (m_currState.vel.x < -m_maxState.vel.x)
				{
					m_currState.vel.x = -m_maxState.vel.x;
				}

				if (m_collided && m_currState.acc.y > 0.0f)
				{
					m_currState.vel.y = 0.0f;
				}

				if (m_currState.acc.y <= 0.0f || m_currState.acc.y < 500.0f)
				{
					m_currState.acc.y += 500.0f * _delta;
				}
				
				// come to a stop
				//if ((m_currState.vel.x > 0 && m_currState.preV.x < 0) || (m_currState.vel.x < 0 && m_currState.preV.x > 0))
				//{
				//	m_currState.acc.x = 0.0f;
				//	m_currState.vel.x = 0.0f;
				//	//m_active = false;
				//}
					
				// Check if at rest
				if (m_currState.vel.x < m_zero && m_currState.vel.x > -m_zero)
				{
					m_currState.acc.x = 0.0f;
					m_currState.vel.x = 0.0f;
					m_stationary = true;
					//m_active = false;
				}

				// change position				
				m_currState.preP = m_currState.pos;
				m_currState.pos += m_currState.vel * _delta; 

				// Player on the platform, update their positions
				if (m_ppCollision && 0 != m_player)
				{
					/*VECTOR2 pos = m_currState.pos - m_currState.preP;
					VECTOR2 player = m_player->GetPosition();
					player.x += pos.x;
					m_player->SetPosition(player);*/
					IDynamicActor* player = reinterpret_cast<IDynamicActor*>(m_player);
					assert(player);
					VECTOR2 vel = player->GetVelocity();
					vel.x = m_currState.vel.x;
					player->SetVelocity(vel);
					}

				// Update bounds
				UpdateBounds();
			}

			virtual void	RenderDebug()
			{
				Renderer::activeRenderer->DrawRect(&m_bounds.rect, m_collided);
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

			virtual void UpdateBounds()
			{
				m_bounds.topLX = m_currState.pos.x - m_bounds.rect.w * 0.5f;
				m_bounds.topLY = m_currState.pos.y - m_bounds.rect.h * 0.5f;
				m_bounds.botRX = m_currState.pos.x + m_bounds.rect.w * 0.5f;
				m_bounds.botRY = m_currState.pos.y + m_bounds.rect.h * 0.5f;
				m_bounds.rect.x = static_cast<Int32>(m_bounds.topLX);
				m_bounds.rect.y = static_cast<Int32>(m_bounds.topLY);
			}

			// Member Variables
		protected:
			State			m_currState;
			State			m_maxState;

			Float32			m_zero;
			Float32			m_mass;

			Bool			m_stationary;
		};
	}
}

#endif // __PAPYRUS_PHYSICS_DYNAMICACTOR_H__