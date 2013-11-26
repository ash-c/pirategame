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

			virtual Bool	Initialise();
			virtual Bool	ShutDown();

			// Member Variables
		protected:

		};
	}
}

#endif // __PAPYRUS_PHYSICS_DYNAMIC_CONTROLLABLE_H__