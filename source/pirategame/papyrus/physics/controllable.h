#pragma once

#ifndef __PAPYRUS_PHYSICS_DYNAMIC_CONTROLLABLE_H__
#define __PAPYRUS_PHYSICS_DYNAMIC_CONTROLLABLE_H__

// Library Includes

// Local Includes
#include "idynamicactor.h"

namespace Papyrus
{
	namespace Physics
	{
		class CControllable : public IDynamicActor
		{
			// Member Functions
		public:
			CControllable();
			virtual ~CControllable();

			virtual Bool	Initialise(VECTOR2 _maxVel, VECTOR2 _maxAcc, VECTOR2 _pos, VECTOR2 _scale, Float32 _mass, EType _type);
			virtual Bool	ShutDown();

			virtual void	Process(Float32 _delta);

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_PHYSICS_DYNAMIC_CONTROLLABLE_H__