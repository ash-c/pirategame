
#pragma once

#ifndef __PAPYRUS_SDLRENDERER_H__
#define __PAPYRUS_SDLRENDERER_H__

// Library Includes

// Local Includes
#include "irenderer.h"

namespace Papyrus
{
	namespace Renderer
	{
		class CSDLRenderer : public IRenderer
		{
			// Member Functions
		public:
			CSDLRenderer();
			virtual ~CSDLRenderer();

			virtual Bool	Initialise(Int32 _width, Int32 _height, Int8* _title, Bool _fullScreen);
			virtual Bool	ShutDown();			

			virtual void	Clear();
			virtual void	Present();

			// Member Variables
		protected:
			SDL_Window*		m_Window;
			SDL_Renderer*	m_Renderer;
		};
	}
}

#endif // __PAPYRUS_SDLRENDERER_H__