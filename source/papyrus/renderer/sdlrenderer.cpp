
// Library Includes
#include <SDL_image.h>
#ifdef PAPYRUS_EDITOR
#include <SDL_syswm.h>
#endif // PAPYRUS_EDITOR

// Local Includes
#include "sdlrenderer.h"
#include "../logging/logger.h"

using namespace Papyrus;

Renderer::CSDLRenderer::CSDLRenderer()
{
}

Renderer::CSDLRenderer::~CSDLRenderer()
{
}

Bool Renderer::CSDLRenderer::Initialise(Int32 _width, Int32 _height, Int8* _title, Bool _fullScreen)
{
	assert(_width >= 800 && _height >= 600 && "Resolution too low.");
	m_Height = _height;
	m_Width = _width;
	m_FullScreen = _fullScreen;

	VALIDATE(PY_SDLASSERT(SDL_Init(SDL_INIT_EVERYTHING), "SDL_Init"));
	
	UInt32 flags = 0;
	if (m_FullScreen)
	{
		flags = SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS;
	}

	m_Window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, flags);
	VALIDATE(PY_SDLASSERT(nullptr == m_Window, "SDL_CreateWindow"));

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
	VALIDATE(PY_SDLASSERT(nullptr == m_Renderer, "SDL_CreateRenderer"));
	
	PY_SDLASSERT(SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0), "SDL_SetRenderDrawColor");
	
	SDL_Surface* icon = IMG_Load("data/icon.png");
	SDL_SetWindowIcon(m_Window, icon);
	SDL_FreeSurface(icon);

	return true;
}

Bool Renderer::CSDLRenderer::ShutDown()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);

	return true;
}

void Renderer::CSDLRenderer::Clear()
{
	PY_SDLASSERT(SDL_RenderClear(m_Renderer), "SDL_RenderClear");
}

void Renderer::CSDLRenderer::Render(SDL_Texture* _tex, SDL_Rect* _dst, SDL_Rect* _clip)
{
	PY_SDLASSERT(SDL_RenderCopy(m_Renderer, _tex, _clip, _dst), "SDL_RenderCopy");
}

Bool Renderer::CSDLRenderer::SetRenderTarget(SDL_Texture* _tex)
{
	VALIDATE(PY_SDLASSERT(SDL_SetRenderTarget(m_Renderer, _tex), "SDL_SetRenderTarget"));
	return true;
}

void Renderer::CSDLRenderer::Present()
{
	SDL_RenderPresent(m_Renderer);
}

Bool Renderer::CSDLRenderer::LoadTexture(Int8* _path, SDL_Texture** _result)
{
	*_result = IMG_LoadTexture(m_Renderer, _path);
	VALIDATE(PY_SDLASSERT(0 == *_result, "IMG_LoadTexture"));
	return true;
}

Bool Renderer::CSDLRenderer::LoadTexture(SDL_Surface* _surface, SDL_Texture** _result)
{
	*_result = SDL_CreateTextureFromSurface(m_Renderer, _surface);
	VALIDATE(PY_SDLASSERT(0 == *_result, "SDL_CreateTextureFromSurface"));
	return true;
}

Bool Renderer::CSDLRenderer::CreateTexture(UInt32 _format, Int32 _access, Int32 _w, Int32 _h, SDL_Texture** _result)
{	
	*_result = SDL_CreateTexture(m_Renderer, _format, _access, _w, _h);
	VALIDATE(PY_SDLASSERT(0 == *_result, "SDL_CreateTexture"));
	return true;
}

Int32 Renderer::CSDLRenderer::GetWidth() const
{
	return m_Width;
}

Int32 Renderer::CSDLRenderer::GetHeight() const
{
	return m_Height;
}

void Renderer::CSDLRenderer::DrawRect(SDL_Rect* _rect, Bool _red)
{
	if (_red)
	{
		SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 0);
		SDL_RenderDrawRect(m_Renderer, _rect);
		SDL_SetRenderDrawColor(m_Renderer, 0, 255, 0, 0);
	}
	else
	{
		SDL_RenderDrawRect(m_Renderer, _rect);
	}
}

#ifdef PAPYRUS_EDITOR
HWND Renderer::CSDLRenderer::GetWindow()
{
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);

	if (SDL_GetWindowWMInfo(m_Window, &info))
	{
		return info.info.win.window;
	}
	else
	{
		return NULL;
	}
}
#endif // PAPYRUS_EDITOR