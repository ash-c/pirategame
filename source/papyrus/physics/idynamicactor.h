#pragma once

#ifndef __PAPYRUS_PHYSICS_DYNAMICACTOR_H__
#define __PAPYRUS_PHYSICS_DYNAMICACTOR_H__

// Library Includes

// Local Includes
#include "iactor.h"
#include "../renderer/renderer.h"
#include "../logging/logger.h"

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
				, m_tileW(25)
				, m_levelW(10000)
				, m_stationary(true)
			{
				m_currState.acc.y = 800.0f; // Gravity
			}

			virtual ~IDynamicActor() {}

			virtual Bool	Initialise(VECTOR2 _maxVel, VECTOR2 _maxAcc, VECTOR2 _pos, VECTOR2 _scale, Float32 _mass, EType _type) = 0;
			virtual Bool	ShutDown() = 0;

			virtual void	SetMass(Float32 _f) { m_mass = _f; }
			virtual Float32	GetMass() { return m_mass; }

			virtual void	SetVelocity(VECTOR2 _v) 
			{
				m_currState.vel = _v; 
				m_currState.preV = _v; 
				//m_currState.acc.y = 0.0f; 
			}
			virtual VECTOR2	GetVelocity() { return m_currState.vel; }

			virtual void	SetYPos(Float32 _y)
			{
				m_renderPos.y = _y;
				m_currState.pos.y = _y;
			}

			virtual void	SetXPos(Float32 _x) 
			{
				m_renderPos.x = _x;
				m_currState.pos.x = _x;
			}

			virtual void	SetPosition(VECTOR2 _v) 
			{ 
				m_renderPos = _v;
				m_currState.pos = _v; 
				UpdateBounds(); 
			} 

			virtual VECTOR2	GetPosition() 
			{ 
				return m_renderPos;
			}

			virtual void	SetScale(VECTOR2 _v)
			{
				m_bounds.rect.h = static_cast<Int32>(_v.y);
				m_bounds.rect.w = static_cast<Int32>(_v.x);
				UpdateBounds();
			}

			virtual void	SetOwner(void* _owner) = 0;
			virtual void* GetOwner() = 0;

			virtual Bool	IsStationary() { return m_stationary; }

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
					}
				}

				// save previous states
				m_currState.preV = m_currState.vel;
				m_currState.preP = m_currState.pos;
					
				// cap x velocity
				if (m_currState.vel.x > m_maxState.vel.x)
				{
					m_currState.vel.x = m_maxState.vel.x;
				} 
				else if (m_currState.vel.x < -m_maxState.vel.x)
				{
					m_currState.vel.x = -m_maxState.vel.x;
				}

				// Drop velocity if collisions
				if (m_vCollision && m_currState.acc.y > 0.0f)
				{
					//m_currState.acc.y = 0.0f;
					m_currState.vel.y = 0.0f;
				}

				if (m_hCollision)
				{
					m_currState.acc.x = 0.0f;
					m_currState.vel.x = 0.0f;
				}

				if (m_currState.acc.y >= 0.0f && m_currState.acc.y < m_maxState.acc.y)
				{
					m_currState.acc.y += 1000.0f * _delta;
				}

				// Check if at rest
				if (m_currState.vel.x < m_zero && m_currState.vel.x > -m_zero)
				{
					m_currState.acc.x = 0.0f;
					m_currState.vel.x = 0.0f;
					m_stationary = true;
				}

				// Player on the platform, update position
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

				// accelerate
				if (!m_vCollision && m_type != Physics::EType::TYPE_PLATFORM)
				{
					m_currState.vel.y += m_currState.acc.y * _delta;
				}
				//if (!m_hCollision && m_type != Physics::EType::TYPE_PLATFORM)
				//{
					m_currState.vel.x += m_currState.acc.x * _delta;
				//}

				// change position	
				//if (!m_hCollision)
				//{
					m_currState.pos.x += m_currState.vel.x * _delta; 
				//}

				if (!m_vCollision)
				{
					m_currState.pos.y += m_currState.vel.y * _delta; 
				}

				// Prevent going off the sides of the screen
				if (m_currState.pos.x <= m_tileW)
				{
					m_currState.pos.x = static_cast<Float32>(m_tileW);
					m_currState.acc.x = 0.0f;
					m_currState.vel.x = 0.0f;
					m_stationary = true;
				}
				if (m_currState.pos.x >= (m_levelW - m_tileW))
				{
					m_currState.pos.x = m_currState.preP.x;
					m_currState.acc.x = 0.0f;
					m_currState.vel.x = 0.0f;
					m_stationary = true;
				}
				Int32 screenH = Renderer::activeRenderer->GetHeight();
				Int32 top = screenH - 2000;
				if (m_currState.pos.y < top + 50)
				{
					m_currState.pos.y = static_cast<Float32>(top + 50);
					m_currState.vel.y = 0.0f;
				}

				// Update bounds
				UpdateBounds();
			}

			virtual void	Interpolate(Float32 _alpha)
			{
				m_renderPos = m_currState.pos * _alpha + m_currState.preP * (1.0f - _alpha);
				if (m_vCollision)
				{
					m_renderPos.y = m_currState.pos.y;
				}
			}

			virtual void	RenderDebug(VECTOR2 _camPos)
			{
				SDL_Rect rect = m_bounds.rect;
				rect.x += static_cast<Int32>(_camPos.x);
				rect.y -= static_cast<Int32>(_camPos.y);
				Renderer::activeRenderer->DrawRect(&rect, m_vCollision || m_hCollision);
			}

			virtual void	SetTileWidth(Int32 _w) { m_tileW = static_cast<Int32>(_w * 0.5f); }

		protected:
			typedef struct _TState
			{
				VECTOR2		pos;	// position
				VECTOR2		vel;	// velocity
				VECTOR2		acc;	// acceleration
				VECTOR2		preV;	// previous velocity
				VECTOR2		preP;	// previous position
			} State;

			void UpdateBounds()
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

			VECTOR2			m_renderPos;

			Float32			m_zero;
			Float32			m_mass;

			Int32			m_tileW;
			Int32			m_levelW;

			Bool			m_stationary;
		};
	}
}

#endif // __PAPYRUS_PHYSICS_DYNAMICACTOR_H__