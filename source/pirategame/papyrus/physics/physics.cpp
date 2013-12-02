
// Library Includes
#include <SDL.h>

// Local Includes
#include "physics.h"
#include "controllable.h"
#include "../logging/logger.h"

using namespace Papyrus;

Physics::IActor**	Physics::actors = 0;
Float32 Physics::m_accumulator = 0.0f;
Int32 Physics::maxActors = 10;

Bool Physics::Initialise()
{
	actors = new IActor*[maxActors];
	assert(actors);
	SDL_memset(actors, 0, sizeof(IActor*) * maxActors);
	
	return true;
}

Bool Physics::ShutDown()
{
	PY_CLEANARRAY(actors, maxActors);

	return true;
}

void Physics::Process(Float32 _frameTime)
{
	// fix the framerate that the physics is calculated at.
	const Float32 dt = 1.0f / 60.0f;
	m_accumulator += _frameTime;

	while (m_accumulator >= dt)
	{
		for (Int16 i = 0; i < maxActors; ++i)
		{
			if (0 != actors[i])
			{
				if (actors[i]->IsActive())
				{
					actors[i]->Process(dt);
				}
			}
		}
		m_accumulator -= dt;
	}

	const Float32 alpha = m_accumulator / dt;
	Logger::WriteToFile("%.4f", alpha);

	for (Int16 i = 0; i < maxActors; ++i)
		{
			if (0 != actors[i])
			{
				if (actors[i]->IsActive())
				{
					actors[i]->ProcessInterpolate(alpha);
				}
			}
		}
}

void Physics::RenderDebug()
{
	for (Int16 i = 0; i < maxActors; ++i)
	{
		if (0 != actors[i])
		{
			actors[i]->RenderDebug();
		}
	}
}

Physics::IStaticActor* Physics::CreateStaticActor()
{
	return 0;
}

Physics::IDynamicActor* Physics::CreateDynamicActor(VECTOR2 _maxVel, VECTOR2 _maxAcc, VECTOR2 _pos, VECTOR2 _scale, Float32 _mass)
{
	IDynamicActor* actor = 0;
	CREATEPOINTER(actor, CControllable);
	assert(actor);
	VALIDATE(actor->Initialise(_maxVel, _maxAcc, _pos, _scale, _mass));
	actors[0] = actor;
	return actor;
}