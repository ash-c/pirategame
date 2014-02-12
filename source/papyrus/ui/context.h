#pragma once

#ifndef __PAPYRUS_UI_CONTEXT_H__
#define __PAPYRUS_UI_CONTEXT_H__

// Library Includes

// Local Includes
#include "interface.h"

class Papyrus::Sprite::ISprite;

namespace Papyrus
{
	namespace UI
	{
		class CContextMenu : public IUIInterface
		{
			// Member Functions
		public:
			CContextMenu();
			virtual ~CContextMenu();

			virtual	Bool	Initialise(Int8* _path);
			virtual Bool	ShutDown();
			virtual void	Render();

			virtual void	Show(VECTOR2* _pos = 0);

			// Member Variables
		protected:
			Papyrus::Sprite::ISprite*	m_background;
			VECTOR2			m_pos;
			
			Int32			m_width;
			Int32			m_height;
		};
	}
}

#endif // __PAPYRUS_UI_CONTEXT_H__