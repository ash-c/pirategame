#pragma once

#ifndef __PAPYRUS_PHYSICS_H__
#define __PAPYRUS_PHYSICS_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "iactor.h"
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

		IStaticActor*	CreateStaticActor(VECTOR2 _pos, VECTOR2 _scale);

		IDynamicActor*	CreateDynamicActor(VECTOR2 _maxVel, VECTOR2 _maxAcc, VECTOR2 _pos, VECTOR2 _scale, Float32 _mass, EType _type);

		void			PlayerStaticCollision(IActor* _actor1, IActor* _actor2);
		void			PlayerPlatformCollision(IActor* _actor1, IActor* _actor2);
		void			StaticPlatformCollision(IActor* _actor1, IActor* _actor2);

		static Int32	ToggleRenderDebug(lua_State* L);

		extern IActor**	actors;

		extern VECTOR2	camPosition;

		extern Float32	m_accumulator;
		extern Int32	maxActors;
		extern Int32	numActors;

		extern Bool		m_renderDebug;
	}
}

#endif // __PAPYRUS_PHYSICS_H__