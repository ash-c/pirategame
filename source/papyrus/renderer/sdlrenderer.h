
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
			virtual void	Render(SDL_Texture* _tex, SDL_Rect* _dst, SDL_Rect* _clip);
			virtual void	Present();

			virtual Bool	LoadTexture(Int8* _path, SDL_Texture** _result);
			virtual Bool	LoadTexture(SDL_Surface* _surface, SDL_Texture** _result);

			virtual Int32	GetWidth() const;
			virtual Int32	GetHeight() const;

			virtual void	DrawRect(SDL_Rect* _rect, Bool _red = false);

			// Member Variables
		protected:
			SDL_Window*		m_Window;
			SDL_Renderer*	m_Renderer;
		};
	}
}

#endif // __PAPYRUS_SDLRENDERER_H__