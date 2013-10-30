
#pragma once

#ifndef __PAPYRUS_CORE_H__
#define __PAPYRUS_CORE_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "../parser/parser.h"
#include "../renderer/renderer.h"
#include "../logging/logger.h"
#include "../sprite/sprite.h"

namespace Papyrus
{
	namespace Core
	{
		Bool		Initialise(Int32 _numParsers);

		Float32		Process();

		void		Render();

		Bool		ShutDown();
	}
}

#endif // __PAPYRUS_CORE_H__