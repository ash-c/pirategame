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

			virtual Bool	Initialise() = 0;
			virtual Bool	ShutDown() = 0;

			virtual void	Process(Float32 _delta) = 0;

			virtual VECTOR2	GetPosition() = 0;

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_PHYSICS_STATICACTOR_H__