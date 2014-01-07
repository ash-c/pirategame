#pragma once

#ifndef __PAPYRUS_UI_INTERFACE_H__
#define __PAPYRUS_UI_INTERFACE_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "../core/utility/refcount.h"

namespace Papyrus
{
	namespace UI
	{
		class IUIInterface : public TRefCount<IUIInterface>
		{
			// Member Functions
		public:
			// Default constructor
			IUIInterface()
			{
			}

			// Default Destructor
			virtual ~IUIInterface()
			{
			}

			/*
			* Initialise the interface.
			*
			* @return		Returns true on successfull intialisation, false otherwise.
			*/
			virtual	Bool	Initialise() = 0;

			/*
			* Cleans memory and shuts the interface down.
			*
			* @return		Returns true on success, false otherwise.
			*/
			virtual Bool	ShutDown() = 0;

			/*
			* Renders this interface to screen.
			*
			* @return		Returns void.
			*/
			virtual void	Render() = 0;

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_UI_INTERFACE_H__