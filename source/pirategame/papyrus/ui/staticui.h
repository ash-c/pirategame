#pragma once

#ifndef __PAPYRUS_UI_STATIC_INTERFACE_H__
#define __PAPYRUS_UI_STATIC_INTERFACE_H__

// Library Includes

// Local Includes
#include "interface.h"

namespace Papyrus
{
	namespace UI
	{
		class CStaticUI : public IUIInterface
		{
			// Member Functions
		public:
			CStaticUI();
			virtual ~CStaticUI();

			virtual Bool		Initialise();
			virtual Bool		ShutDown();
			virtual void		Render();

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_UI_STATIC_INTERFACE_H__