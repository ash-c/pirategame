#pragma once

#ifndef __PAPYRUS_PHYSICS_STATICACTOR_H__
#define __PAPYRUS_PHYSICS_STATICACTOR_H__

// Library Includes

// Local Includes
#include "iactor.h"

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

			virtual Bool	Initialise(VECTOR2 _maxVel, VECTOR2 _pos, Float32 _mass) = 0;
			virtual Bool	ShutDown() = 0;

			virtual void	Process(Float32 _frameTime) = 0;

			virtual VECTOR2	GetPosition() = 0;

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_PHYSICS_STATICACTOR_H__