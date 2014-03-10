
#pragma once

#ifndef __PAPYRUS_CORE_H__
#define __PAPYRUS_CORE_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "../core/utility/timer.h"
#include "../parser/parser.h"
#include "../renderer/renderer.h"
#include "../logging/logger.h"
#include "../sprite/sprite.h"
#include "../input/input.h"
#include "../physics/physics.h"
#include "../ui/ui.h"

namespace Papyrus
{
	namespace Core
	{
		Bool		Initialise();

		Float32		Process();

		void		Render();

		Bool		ShutDown();

		void		Pause();

		Bool		IsPaused();
		
		extern Timer::CTimer* timer;
	}
}

#endif // __PAPYRUS_CORE_H__