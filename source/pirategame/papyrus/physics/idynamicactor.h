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
			IDynamicActor() {}

			virtual ~IDynamicActor() {}

			virtual Bool	Initialise() = 0;
			virtual Bool	ShutDown() = 0;

			virtual void	Process(Float32 _delta) = 0;

			virtual VECTOR2	GetPosition() { return m_state.pos; }

		protected:
			typedef struct _TState
			{
				VECTOR2		pos;	// position
				VECTOR2		vel;	// velocity
			} State;

			typedef struct _TDerivative
			{
				VECTOR2		dx;		// Derivative of position:velocity
				VECTOR2		dy;		// Derivative of velocity:acceleration
			} Derivative;

			virtual Derivative Evaluate(const State& _initial, Float32 _t, Float32 _dt, const Derivative& d)
			{
				State state;
				state.pos = _initial.pos + d.dx * _dt;
			}

			// Member Variables
		protected:

			State			m_state;
			Derivative		m_der;
		};
	}
}

#endif // __PAPYRUS_PHYSICS_DYNAMICACTOR_H__