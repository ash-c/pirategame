#pragma once

#ifndef __PAPYRUS_PHYSICS_STATIC_H__
#define __PAPYRUS_PHYSICS_STATIC_H__

// Library Includes

// Local Includes
#include "istaticactor.h"

namespace Papyrus
{
	namespace Physics
	{
		class CStatic : public IStaticActor
		{
			// Member Functions
		public:
			CStatic();
			virtual ~CStatic();

			virtual Bool	Initialise(VECTOR2 _pos, VECTOR2 _scale);
			virtual Bool	ShutDown();

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_PHYSICS_STATIC_H__