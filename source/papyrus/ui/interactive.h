#pragma once

#ifndef __PAPYRUS_UI_INTERACTIVE_UI_H__
#define __PAPYRUS_UI_INTERACTIVE_UI_H__

// Library Includes

// Local Includes
#include "interface.h"

namespace Papyrus
{
	namespace UI
	{
		class CInteractiveUI : public IUIInterface
		{
			// Member Functions
		public:
			CInteractiveUI();
			virtual ~CInteractiveUI();

			virtual Bool		Initialise(Int8* _path);
			virtual Bool		ShutDown();
			virtual void		Render();

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_UI_INTERACTIVE_UI_H__