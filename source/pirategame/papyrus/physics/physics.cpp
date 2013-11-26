
// Library Includes
#include <SDL.h>

// Local Includes
#include "physics.h"
#include "controllable.h"

using namespace Papyrus;

Physics::IActor**	Physics::actors = 0;

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
	for (Int16 i = 0; i < maxActors; ++i)
	{
		if (0 != actors[i])
		{
			if (actors[i]->IsActive())
			{
				actors[i]->Process(_frameTime);
			}
		}
	}
}

Physics::IStaticActor* Physics::CreateStaticActor()
{
	return 0;
}

Physics::IDynamicActor* Physics::CreateDynamicActor()
{
	IDynamicActor* actor = 0;
	CREATEPOINTER(actor, CControllable);
	assert(actor);
	VALIDATE(actor->Initialise());
	actors[0] = actor;
	return actor;
}