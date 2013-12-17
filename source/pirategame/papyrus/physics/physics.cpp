
// Library Includes
#include <SDL.h>

// Local Includes
#include "physics.h"
#include "controllable.h"
#include "static.h"
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
	// process physics objects
	// fix the framerate that the physics is calculated at.
	const Float32 dt = 1.0f / 60.0f; // 60FPS
	m_accumulator += _frameTime;

	while (m_accumulator >= dt)
	{
		for (Int16 i = 0; i < maxActors; ++i)
		{
			if (0 != actors[i])
			{
				//if (actors[i]->IsActive())
				//{
					actors[i]->Process(dt);
				//}
			}
		}

		// reset
		for (Int16 i = 0; i < maxActors; ++i)
		{
			if (0 != actors[i])
			{
				actors[i]->SetCollided(false);
			}
		}

		// collision detection
		for (Int16 i = 0; i < maxActors; ++i)
		{
			if (0 != actors[i])
			{
				for (Int16 j = i; j < maxActors; ++j)
				{			
					if (0 != actors[j])
					{			
						if (actors[i] != actors[j])
						{
							SDL_Rect result;
							if (SDL_IntersectRect(&actors[i]->GetRect(), &actors[j]->GetRect(), &result))
							{
								VECTOR2 pos = actors[i]->GetPosition();
								// clamp values
								pos.x = static_cast<Float32>(static_cast<Int32>(pos.x));
								pos.y = static_cast<Float32>(static_cast<Int32>(pos.y));

								// collision!
								actors[i]->SetCollided(true);
								actors[j]->SetCollided(true);
								Bool bYCollision = false;

								if (result.y < pos.y) // below
								{
									if ((result.y + 50 + result.h) > pos.y)
									{
										if ((pos.y + result.h) > result.y)
										{
											pos.y += result.h;
										}
										actors[i]->SetPosition(VECTOR2(pos.x, pos.y));
									}
									bYCollision = true;
								}
								else if (result.y > pos.y) // above
								{
									if (result.y - 50 < pos.y)
									{
										if ((pos.y - result.h) < result.y)
										{
											pos.y -= result.h;
										}
										actors[i]->SetPosition(VECTOR2(pos.x, pos.y));
									}
									bYCollision = true;
								}

								if (result.x > pos.x && !bYCollision) // left
								{
									pos.x -= result.w;
									actors[i]->SetPosition(VECTOR2(pos.x, pos.y));
								}
								else if (result.x < pos.x && !bYCollision) // right
								{
									pos.x += result.w;
									actors[i]->SetPosition(VECTOR2(pos.x, pos.y));
								}
							}
						}
					}
				}
			}
		}
		m_accumulator -= dt;
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

Physics::IStaticActor* Physics::CreateStaticActor(VECTOR2 _pos, VECTOR2 _scale)
{
	IStaticActor* actor = 0;

	for (Int16 i = 0; i < maxActors; ++i)
	{
		if (0 == actors[i])
		{
			CREATEPOINTER(actor, CStatic);
			assert(actor);
			VALIDATE(actor->Initialise(_pos, _scale));
			actors[i] = actor;
			return actor;
		}
	}

	Logger::Write("Not enough room to create new static actor. Increase maxActors.");
	return actor;
}

Physics::IDynamicActor* Physics::CreateDynamicActor(VECTOR2 _maxVel, VECTOR2 _maxAcc, VECTOR2 _pos, VECTOR2 _scale, Float32 _mass, EType _type)
{
	IDynamicActor* actor = 0;

	for (Int16 i = 0; i < maxActors; ++i)
	{
		if (0 == actors[i])
		{
			CREATEPOINTER(actor, CControllable);
			assert(actor);
			VALIDATE(actor->Initialise(_maxVel, _maxAcc, _pos, _scale, _mass, _type));
			actors[i] = actor;
			return actor;
		}
	}

	Logger::Write("Not enough room to create new dynamic actor. Increase maxActors.");
	return actor;
}