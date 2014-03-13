#pragma once

#ifndef __PAPYRUS_UI_INTERACTIVE_UI_H__
#define __PAPYRUS_UI_INTERACTIVE_UI_H__

// Library Includes

// Local Includes
#include "interface.h"
#include "../input/input.h"

namespace Papyrus
{
	namespace UI
	{
		class CInteractiveUI : public IUIInterface, Input::IInputObserver
		{
			// Member Functions
		public:
			CInteractiveUI();
			virtual ~CInteractiveUI();

			virtual Bool		Initialise(Int8* _path);
			virtual Bool		ShutDown();
			virtual void		Process(Float32 _delta);
			virtual void		Render();

			virtual Bool		Toggle();

			virtual void		Notify(SDL_Event* _e);

			// Member Variables
		protected:
			// These variables are used for controllers navigating buttons
			Int32				m_firstButton;
			Int32				m_activeButton;
		};
	}
}

#endif // __PAPYRUS_UI_INTERACTIVE_UI_H__