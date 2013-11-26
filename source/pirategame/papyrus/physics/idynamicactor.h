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
				: m_accumulator(0.0f)
				, m_time(0.0f)
			{
			}

			virtual ~IDynamicActor() {}

			virtual Bool	Initialise() = 0;
			virtual Bool	ShutDown() = 0;
			virtual void	SetVelocity(VECTOR2 _v) { m_currState.vel = _v; }
			virtual void	SetPosition(VECTOR2 _v) { m_currState.pos = _v; m_prevState.pos = _v; } 
			virtual VECTOR2	GetPosition() { return m_currState.pos; }

			virtual void	Process(Float32 _frameTime)
			{
				const Float32 dt = 0.001f;
				if (_frameTime > 1.0f/60.0f)
				{
					_frameTime = 1.0f/60.0f;
				}

				m_accumulator += _frameTime;

				while (m_accumulator >= dt)
				{
					m_prevState = m_currState;
					Integrate(m_currState, m_time, dt);
					m_time += dt;
					m_accumulator -= dt;
				}
				//m_time = 0.0f;

				const Float32 alpha = m_accumulator / dt;

				m_currState = m_currState * alpha + m_prevState * (1.0f - alpha);
			}

		protected:
			typedef struct _TState
			{
				const _TState operator* (const Float32 _f) 
				{
					_TState state = *this;
					state.pos *= _f;
					state.vel *= _f;
					return state;
				}

				const _TState operator+ (const _TState _t) const
				{
					_TState state = *this;
					state.pos += _t.pos;
					state.vel += _t.vel;
					return state;
				}

				VECTOR2		pos;	// position
				VECTOR2		vel;	// velocity
			} State;

			typedef struct _TDerivative
			{
				VECTOR2		dx;		// Derivative of position:velocity
				VECTOR2		dv;		// Derivative of velocity:acceleration
			} Derivative;

			virtual		Derivative Evaluate(const State& _initial, Float32 _t)
			{
				Derivative output;
				output.dx = _initial.vel;
				output.dv = Acceleration(_initial, _t);
				return output;
			}

			virtual		Derivative Evaluate(const State& _initial, Float32 _t, Float32 _dt, const Derivative& d)
			{
				State state;
				state.pos = _initial.pos + d.dx * _dt;
				state.vel = _initial.vel + d.dv * _dt;

				Derivative output;
				output.dx = state.vel;
				output.dv = Acceleration(state, _t + _dt);
				return output;
			}

			VECTOR2		Acceleration(const State& _state, Float32 _t)
			{
				const Float32 k = 10.0f;
				const Float32 b = 1.0f;
				return (_state.pos * -k) - (_state.vel * b);
			}

			void		Integrate(State& _state, Float32 _t, Float32 _dt)
			{
				Derivative a = Evaluate(_state, _t);
				Derivative b = Evaluate(_state, _t, _dt * 0.5f, a);
				Derivative c = Evaluate(_state, _t, _dt * 0.5f, b);
				Derivative d = Evaluate(_state, _t, _dt, c);

				const VECTOR2 dxdt = (a.dx + (b.dx  + c.dx) * 2.0f + d.dx) * 1.0f / 6.0f;
				const VECTOR2 dvdt = (a.dv + (b.dv  + c.dv) * 2.0f + d.dv) * 1.0f / 6.0f;

				_state.pos = _state.pos + dxdt * _dt;
				_state.vel = _state.vel + dvdt * _dt;
			}

			// Member Variables
		protected:
			State			m_prevState;
			State			m_currState;

			Float32			m_accumulator;
			Float32			m_time;
		};
	}
}

#endif // __PAPYRUS_PHYSICS_DYNAMICACTOR_H__