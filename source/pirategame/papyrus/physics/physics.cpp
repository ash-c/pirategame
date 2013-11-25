
// Library Includes
#include <SDL.h>

// Local Includes
#include "physics.h"

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

Physics::IStaticActor* Physics::CreateStaticActor()
{
	return 0;
}

Physics::IDynamicActor* Physics::CreateDynamicActor()
{
	return 0;
}