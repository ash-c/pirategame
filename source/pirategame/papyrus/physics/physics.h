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

		void			Process(Float32 _frameTime);
		void			RenderDebug();

		IStaticActor*	CreateStaticActor();

		IDynamicActor*	CreateDynamicActor(VECTOR2 _maxVel, VECTOR2 _maxAcc, VECTOR2 _pos, VECTOR2 _scale, Float32 _mass);


		extern IActor**	actors;

		extern Float32	m_accumulator;
		extern Int32	maxActors;
	}
}

#endif // __PAPYRUS_PHYSICS_H__