
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
Bool Physics::m_renderDebug = false;

Bool Physics::Initialise()
{
	actors = new IActor*[maxActors];
	assert(actors);
	SDL_memset(actors, 0, sizeof(IActor*) * maxActors);

	lua_register(Logger::luaState, "ToggleRenderDebug", ToggleRenderDebug);
	
	return true;
}

Bool Physics::ShutDown()
{
	PY_CLEANARRAY(actors, maxActors);

	return true;
}

void Physics::Process(Float32 _frameTime)
{

	for (UInt16 i = 0; i < numActors; ++i)
	{
		actors[i]->SetHCollided(false);
		actors[i]->SetVCollided(false);
		actors[i]->SetPPCollided(0, false);
	}

	// collision detection
	for (Int16 i = 0; i < numActors; ++i)
	{
		for (Int16 j = i + 1; j < numActors; ++j)
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

	// process physics objects
	// fix the framerate that the physics is calculated at.
	const Float32 dt = 1.0f / 60.0f; // 60FPS
	m_accumulator += _frameTime;

	while (m_accumulator >= dt)
	{	
		for (Int16 i = 0; i < numActors; ++i)
		{
			if (actors[i]->IsActive())
			{
				actors[i]->Process(dt);
			}
		}

		m_accumulator -= dt;
	}

	Float32 alpha = m_accumulator / dt;

	for (UInt16 i = 0; i < numActors; ++i)
	{
		if (actors[i]->IsActive())
		{
			actors[i]->Interpolate(alpha);
		}
	}
}

void Physics::RenderDebug()
{
	if (m_renderDebug)
	{
		for (Int16 i = 0; i < numActors; ++i)
		{
			if (0 != actors[i])
			{
				if (actors[i]->IsActive()) actors[i]->RenderDebug(camPosition);
			}
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
	// actor1 is always the player
	// actor2 is always the static physics object
	SDL_Rect result;
	SDL_Rect rect1 = _actor1->GetRect();
	SDL_Rect rect2 = _actor2->GetRect();
	if (SDL_IntersectRect(&rect1, &rect2, &result))
	{
		VECTOR2 pos = _actor1->GetPosition();
		VECTOR2 pos2 = _actor2->GetPosition();
		
		// vertical collision
		if (pos.x > rect2.x && pos.x < rect2.x + rect2.w) 
		{
			if (pos2.y < pos.y) // below
			{
				pos.y = pos2.y + rect2.h * 0.5f + rect1.h * 0.5f;
				_actor1->SetYPos(pos.y);
				_actor1->SetVCollided(true);
				_actor2->SetVCollided(true);
			}
			else if (pos2.y > pos.y) // above
			{
				pos.y = pos2.y - rect2.h * 0.5f - rect1.h * 0.5f + 1.0f;
				_actor1->SetYPos(pos.y);
				_actor1->SetVCollided(true);
				_actor2->SetVCollided(true);
			}
		}

		// horizontal collision
		if (pos.y > rect2.y && pos.y < rect2.y + rect2.h)
		{
			if (pos2.x > pos.x) // left
			{
				pos.x = rect2.x - rect1.w * 0.5f;
				_actor1->SetXPos(pos.x);
				_actor1->SetHCollided(true);
				_actor2->SetHCollided(true);
			}
			else if (pos2.x < pos.x) // right
			{
				pos.x = rect2.x + rect2.w + rect1.w * 0.5f;
				_actor1->SetXPos(pos.x);
				_actor1->SetHCollided(true);
				_actor2->SetHCollided(true);
			}
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
		_actor1->SetVCollided(true);
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
		_actor1->SetHCollided(true);
		_actor2->SetHCollided(true);
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

Int32 Physics::ToggleRenderDebug(lua_State* L)
{
	m_renderDebug = !m_renderDebug;
	return 0;
}