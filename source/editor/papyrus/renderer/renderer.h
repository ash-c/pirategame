
#pragma once

#ifndef __PAPYRUS_RENDERER_H__
#define __PAPYRUS_RENDERER_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "irenderer.h"

namespace Papyrus
{
	namespace Renderer
	{
		Bool	Initialise(Int32 _width, Int32 _height, Int8* _title, Bool _fullScreen);
		Bool	ShutDown();

		void	Clear();
		void	Present();

		extern IRenderer* activeRenderer;
	}
}

#endif // __PAPYRUS_RENDERER_H__