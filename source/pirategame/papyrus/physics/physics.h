#pragma once

#ifndef __PAPYRUS_PHYSICS_H__
#define __PAPYRUS_PHYSICS_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "istaticactor.h"
#include "idynamicactor.h"

namespace Papyrus
{
	namespace Physics
	{
		Bool			Initialise();

		Bool			ShutDown();

		IStaticActor*	CreateStaticActor();

		IDynamicActor*	CreateDynamicActor();


		extern IActor**	actors;

		extern Int32 maxActors;
	}
}

#endif // __PAPYRUS_PHYSICS_H__