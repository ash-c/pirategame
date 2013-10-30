
#pragma once

#ifndef __PAPYRUS_LOGGER_ILOGTYPE_H__
#define __PAPYRUS_LOGGER_ILOGTYPE_H__

// Library Includes

// Local Includes
#include "../core/utility/refcount.h"

namespace Papyrus
{
	namespace Logger
	{
		class ILogTarget : public TRefCount<ILogTarget>
		{
			// Member Functions
		public:
			// Default Constructor
			ILogTarget()
			{
			}

			// Default Destructor
			virtual ~ILogTarget()
			{
			}

			virtual Bool		Initialise() = 0;
			virtual Bool		ShutDown() = 0;

			virtual void		Process(Float32 _fDelta) = 0;
			virtual void		Render() = 0;

			virtual void		Write(Int8* _msg) = 0;

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_LOGGER_ILOGTYPE_H__