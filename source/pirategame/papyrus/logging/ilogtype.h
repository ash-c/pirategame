
#pragma once

#ifndef __PAPYRUS_LOGGER_ILOGTYPE_H__
#define __PAPYRUS_LOGGER_ILOGTYPE_H__

// Library Includes
#include <SDL.h>

// Local Includes
#include "../core/utility/refcount.h"

namespace Papyrus
{
	namespace Logger
	{		
		typedef enum _ETrackType
		{
			INVALID_TRACKTYPE = INVALID_ID,
			TRACKTYPE_BOOL,
			TRACKTYPE_CHAR,
			TRACKTYPE_INT,
			TRACKTYPE_UINT,
			TRACKTYPE_FLOAT,
			TRACKTYPE_VECTOR3,
			TRACKTYPE_VECTOR4,
			MAX_TRACKTYPE
		} ETrackType;

		class ILogType : public TRefCount<ILogType>
		{
			// Member Functions
		public:
			// Default Constructor
			ILogType()
			{
			}

			// Default Destructor
			virtual ~ILogType()
			{
			}

			virtual Bool		Initialise(const Int8* _path = 0) = 0;
			virtual Bool		ShutDown() = 0;

			virtual void		Process(Float32 _fDelta) { }
			virtual void		Render()  { }

			virtual void		Write(Int8* _msg) = 0;

			virtual Bool		Toggle() { return true; }
			virtual void		TrackValue(void* _value, ETrackType _type, const Int8* _tag) { }			
			virtual void		StopTracking(const Int8* _tag) { }

			virtual void Input(SDL_Event _e) { };

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_LOGGER_ILOGTYPE_H__