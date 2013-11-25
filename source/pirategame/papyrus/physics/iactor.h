#pragma once

#ifndef __PAPYRUS_PHYSICS_ACTOR_H__
#define __PAPYRUS_PHYSICS_ACTOR_H__

// Library Includes

// Local Includes
#include "../core/utility/refcount.h"

namespace Papyrus
{
	namespace Physics
	{
		class IActor : public TRefCount<IActor>
		{
			// Member Functions
		public:
			IActor() {}

			virtual ~IActor() {}

			virtual Bool	Initialise() = 0;
			virtual Bool	ShutDown() = 0;

			virtual void	Process(Float32 _delta) = 0;

			virtual VECTOR2	GetPosition() = 0;

			// Member Variables
		protected:
			typedef struct _TAABB
			{
				Float32		topLX;
				Float32		topLY;
				Float32		botRX;
				Float32		botRY;
			} AABB;

			AABB			m_bounds;
		};
	}
}

#endif // __PAPYRUS_PHYSICS_ACTOR_H__