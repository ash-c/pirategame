
#pragma once

#ifndef __PAPYRUS_IRENDERER_H__
#define __PAPYRUS_IRENDERER_H__

// Library Includes
#include <SDL.h>

// Local Includes
#include "../core/utility/refcount.h"

namespace Papyrus
{
	namespace Renderer
	{
		class IRenderer : public TRefCount<IRenderer>
		{
			// Member Functions
		public:
			// Default Constructor
			IRenderer()
				: m_Width(1920)
				, m_Height(1080)
				, m_Title(0)
				, m_FullScreen(true)
			{
			}
			// Default Destructor
			virtual ~IRenderer()
			{
			}

			virtual Bool	Initialise(Int32 _width, Int32 _height, Int8* _title, Bool _fullScreen) = 0;
			virtual Bool	ShutDown() = 0;			

			virtual void	Clear() = 0;
			virtual void	Render(SDL_Texture* _tex, SDL_Rect* _dst, SDL_Rect* _clip) = 0;
			virtual void	Present() = 0;

			virtual Bool	LoadTexture(Int8* _path, SDL_Texture** _result) = 0;
			virtual Bool	LoadTexture(SDL_Surface* _surface, SDL_Texture** _result) = 0;

			virtual Int32	GetWidth() const = 0;
			virtual Int32	GetHeight() const = 0;

			virtual void	DrawRect(SDL_Rect* _rect) = 0;

			// Member Variables
		protected:
			Int32			m_Width;
			Int32			m_Height;
			Int8*			m_Title;
			Bool			m_FullScreen;
		};
	}
}

#endif // __PAPYRUS_IRENDERER_H__