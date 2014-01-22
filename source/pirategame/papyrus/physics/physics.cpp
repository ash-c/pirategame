
// Library Includes
#include <SDL.h>

// Local Includes
#include "physics.h"
#include "controllable.h"
#include "static.h"
#include "../logging/logger.h"

using namespace Papyrus;

Physics::IActor**	Physics::actors = 0;
VECTOR2 Physics::camPosition;
Float32 Physics::m_accumulator = 0.0f;
Int32 Physics::maxActors = 10;
Int32 Physics::numActors = 0;

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
		for (Int16 i = 0; i < numActors; ++i)
		{
			if (0 != actors[i])
			{
				if (actors[i]->IsActive())
				{
					actors[i]->Process(dt);
				}

				actors[i]->SetCollided(false);
				actors[i]->SetPPCollided(0, false);
			}
		}

		// collision detection
		for (Int16 i = 0; i < numActors; ++i)
		{
			if (0 != actors[i])
			{
				for (Int16 j = i + 1; j < numActors; ++j)
				{			
					if (0 != actors[j])
					{			
						if (actors[i] != actors[j] && actors[i]->IsActive() && actors[j]->IsActive())
						{
							Physics::EType type1 = actors[i]->GetType();
							Physics::EType type2 = actors[j]->GetType();

							if (type1 == Physics::EType::TYPE_PLAYER && type2 == Physics::EType::TYPE_STATIC)
							{
								PlayerStaticCollision(actors[i], actors[j]);
							}
							else if (type2 == Physics::EType::TYPE_PLAYER && type1 == Physics::EType::TYPE_STATIC)
							{
								PlayerStaticCollision(actors[j], actors[i]);
							}
							else if (type1 == Physics::EType::TYPE_PLATFORM && type2 == Physics::EType::TYPE_STATIC)
							{
								StaticPlatformCollision(actors[i], actors[j]);
							}
							else if (type1 == Physics::EType::TYPE_STATIC && type2 == Physics::EType::TYPE_PLATFORM)
							{
								StaticPlatformCollision(actors[j], actors[i]);
							}
							else if (type1 == Physics::EType::TYPE_PLAYER && type2 == Physics::EType::TYPE_PLATFORM)
							{
								PlayerPlatformCollision(actors[i], actors[j]);
							}
							else if (type1 == Physics::EType::TYPE_PLATFORM && type2 == Physics::EType::TYPE_PLAYER)
							{
								PlayerPlatformCollision(actors[j], actors[i]);
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
	for (Int16 i = 0; i < numActors; ++i)
	{
		if (0 != actors[i])
		{
			if (actors[i]->IsActive()) actors[i]->RenderDebug(camPosition);
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
			++numActors;
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
			++numActors;
			return actor;
		}
	}

	Logger::Write("Not enough room to create new dynamic actor. Increase maxActors.");
	return actor;
}

void Physics::PlayerStaticCollision(IActor* _actor1, IActor* _actor2)
{
	SDL_Rect result;
	if (SDL_IntersectRect(&_actor1->GetRect(), &_actor2->GetRect(), &result))
	{
		VECTOR2 pos = _actor1->GetPosition();
		// clamp values
		pos.x = static_cast<Float32>(static_cast<Int32>(pos.x));
		pos.y = static_cast<Float32>(static_cast<Int32>(pos.y));

		// collision!
		_actor1->SetCollided(true);
		_actor2->SetCollided(true);
		Bool bYCollision = false;

		if (result.y < pos.y) // below
		{
			if ((result.y + 50 + result.h) > pos.y)
			{
				if ((pos.y + result.h) > result.y)
				{
					pos.y += result.h;
				}
				_actor1->SetPosition(VECTOR2(pos.x, pos.y));
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
				_actor1->SetPosition(VECTOR2(pos.x, pos.y));
			}
			bYCollision = true;
		}

		if (result.x > pos.x && !bYCollision) // left
		{
			pos.x -= result.w;
			_actor1->SetPosition(VECTOR2(pos.x, pos.y));
		}
		else if (result.x < pos.x && !bYCollision) // right
		{
			pos.x += result.w;
			_actor1->SetPosition(VECTOR2(pos.x, pos.y));
		}
	}
}

void Physics::PlayerPlatformCollision(IActor* _actor1, IActor* _actor2)
{
	SDL_Rect result;
	if (SDL_IntersectRect(&_actor1->GetRect(), &_actor2->GetRect(), &result))
	{
		VECTOR2 pos = _actor1->GetPosition();
		// clamp values
		pos.x = static_cast<Float32>(static_cast<Int32>(pos.x));
		pos.y = static_cast<Float32>(static_cast<Int32>(pos.y));

		// collision!
		_actor1->SetCollided(true);
		Bool bYCollision = false;

		if (result.y < pos.y) // below
		{
			if ((result.y + 50 + result.h) > pos.y)
			{
				if ((pos.y + result.h) > result.y)
				{
					pos.y += result.h;
				}
				_actor1->SetPosition(VECTOR2(pos.x, pos.y));
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
				_actor1->SetPosition(VECTOR2(pos.x, pos.y));
				_actor2->SetPPCollided(_actor1, true);
				_actor1->SetPPCollided(0, true);
			}
			bYCollision = true;
		}

		if (result.x > pos.x && !bYCollision) // left
		{
			pos.x -= result.w;
			_actor1->SetPosition(VECTOR2(pos.x, pos.y));
		}
		else if (result.x < pos.x && !bYCollision) // right
		{
			pos.x += result.w;
			_actor1->SetPosition(VECTOR2(pos.x, pos.y));
		}
	}
}

void Physics::StaticPlatformCollision(IActor* _actor1, IActor* _actor2)
{
	SDL_Rect result;
	if (SDL_IntersectRect(&_actor1->GetRect(), &_actor2->GetRect(), &result))
	{
		VECTOR2 pos = _actor1->GetPosition();
		// clamp values
		pos.x = static_cast<Float32>(static_cast<Int32>(pos.x));
		pos.y = static_cast<Float32>(static_cast<Int32>(pos.y));

		// collision!
		_actor1->SetCollided(true);
		_actor2->SetCollided(true);
		Bool bYCollision = false;

		if (result.x > pos.x && !bYCollision) // left
		{
			pos.x -= result.w;
			_actor1->SetPosition(VECTOR2(pos.x, pos.y));
		}
		else if (result.x < pos.x && !bYCollision) // right
		{
			pos.x += result.w;
			_actor1->SetPosition(VECTOR2(pos.x, pos.y));
		}
	}
}