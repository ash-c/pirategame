
// Library Includes
#include <SDL.h>

// Local Includes
#include "physics.h"
#include "controllable.h"
#include "static.h"
#include "../logging/logger.h"

#include "../../pirategame/character/character.h"

using namespace Papyrus;
using namespace Papyrus::Physics;

Physics::IActor**	Physics::actors = 0;
VECTOR2 Physics::camPosition;
Float32 Physics::m_accumulator = 0.0f;
Int32 Physics::maxActors = 10;
Int32 Physics::numActors = 0;
#ifdef _DEBUG
Bool Physics::m_renderDebug = true;
#else 
Bool Physics::m_renderDebug = false;
#endif // _DEBUG

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
		actors[i]->SetPECollided(false);
		actors[i]->SetPWCollided(false);
		actors[i]->SetPCCollided(false);
	}

	// process physics objects
	// fix the framerate that the physics is calculated at.
	const Float32 dt = 1.0f / 60.0f; // 60FPS
	m_accumulator += _frameTime;

	while (m_accumulator >= dt)
	{	

	// collision detection
	for (Int16 i = 0; i < numActors; ++i)
	{
		for (Int16 j = i + 1; j < numActors; ++j)
		{					
			if (actors[i] != actors[j] && actors[i]->IsActive() && actors[j]->IsActive())
			{
				Physics::EType type1 = actors[i]->GetType();
				Physics::EType type2 = actors[j]->GetType();

				if (type1 == EType::TYPE_PLAYER && type2 == EType::TYPE_STATIC)
				{
					PlayerStaticCollision(actors[i], actors[j]);
				}
				else if (type1 == EType::TYPE_STATIC && type2 == EType::TYPE_PLAYER)
				{
					PlayerStaticCollision(actors[j], actors[i]);
				}
				else if (type1 == EType::TYPE_BASIC_ENEMY && type2 == EType::TYPE_STATIC)
				{
					PlayerStaticCollision(actors[i], actors[j]);
				}
				else if (type1 == EType::TYPE_STATIC && type2 == EType::TYPE_BASIC_ENEMY)
				{
					PlayerStaticCollision(actors[j], actors[i]);
				}
				else if (type1 == EType::TYPE_PLAYER && type2 == EType::TYPE_BASIC_ENEMY)
				{
					PlayerEnemyCollision(actors[i], actors[j]);
				}
				else if (type1 == EType::TYPE_BASIC_ENEMY && type2 == EType::TYPE_PLAYER)
				{
					PlayerEnemyCollision(actors[j], actors[i]);
				}
				else if (type1 == EType::TYPE_PLATFORM && type2 == EType::TYPE_STATIC)
				{
					StaticPlatformCollision(actors[i], actors[j]);
				}
				else if (type1 == EType::TYPE_STATIC && type2 == EType::TYPE_PLATFORM)
				{
					StaticPlatformCollision(actors[j], actors[i]);
				}
				else if (type1 == EType::TYPE_PLAYER && type2 == EType::TYPE_PLATFORM)
				{
					PlayerPlatformCollision(actors[i], actors[j]);
				}
				else if (type1 == EType::TYPE_PLATFORM && type2 == EType::TYPE_PLAYER)
				{
					PlayerPlatformCollision(actors[j], actors[i]);
				}
				else if (type1 == EType::TYPE_PLAYER && type2 == EType::TYPE_WATER)
				{
					PlayerWaterCollision(actors[i], actors[j]);
				}
				else if (type1 == EType::TYPE_WATER && type2 == EType::TYPE_PLAYER)
				{
					PlayerWaterCollision(actors[j], actors[i]);
				}
				else if (type1 == EType::TYPE_PLAYER && type2 == EType::TYPE_COIN)
				{
					PlayerCoinCollision(actors[j], actors[i]);
				}
				else if (type1 == EType::TYPE_COIN && type2 == EType::TYPE_PLAYER)
				{
					PlayerCoinCollision(actors[j], actors[i]);
				}
			}
		}
	}

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

Bool Physics::FlushActor(IActor* _actor)
{
	if (0 != _actor)
	{
		for (Int32 i = 0; i < numActors; ++i)
		{
			if (_actor == actors[i])
			{
				PY_DELETE_RELEASE(actors[i]);
				--numActors;

				for (Int32 k = i; k < (numActors); ++k)
				{
					actors[k] = actors[k + 1];
					if (0 == actors[k]) break;
				}
				actors[numActors] = 0;
				return true;
			}
		}
	}

	return false;
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
	SDL_Rect rect1 = _actor1->GetRect();
	SDL_Rect rect2 = _actor2->GetRect();
	if (SDL_IntersectRect(&_actor1->GetRect(), &_actor2->GetRect(), &result))
	{
		VECTOR2 pos = _actor1->GetPosition();
		VECTOR2 pos2 = _actor2->GetPosition();

		// collision!
		_actor1->SetVCollided(true);
		Bool bYCollision = false;

		// vertical
		if (pos.x > rect2.x && pos.x < rect2.x + rect2.w) 
		{
			if (pos2.y < pos.y) // below
			{
				pos.y = pos2.y + rect2.h * 0.5f + rect1.h * 0.5f;
				_actor1->SetPosition(VECTOR2(pos.x, pos.y));
				bYCollision = true;
			}		
			else if (pos2.y > pos.y) // above
			{
				pos.y = pos2.y - rect2.h * 0.5f - rect1.h * 0.5f + 1.0f;
				_actor1->SetPosition(VECTOR2(pos.x, pos.y));
				_actor2->SetPPCollided(_actor1, true);
				_actor1->SetPPCollided(0, true);
				bYCollision = true;
			}
		}

		// horizontal
		if (pos.y > rect2.y && pos.y < rect2.y + rect2.h)
		{
			if (pos2.x > pos.x) // left
			{
				pos.x = rect2.x - rect1.w * 0.5f;
				_actor1->SetPosition(VECTOR2(pos.x, pos.y));
			}
			else if (pos2.x < pos.x) // right
			{
				pos.x = rect2.x + rect2.w + rect1.w * 0.5f;
				_actor1->SetPosition(VECTOR2(pos.x, pos.y));
			}
		}
	}
}

void Physics::PlayerEnemyCollision(IActor* _actor1, IActor* _actor2)
{
	// actor1 is always the player
	// actor2 is always the enemy
	SDL_Rect result;
	if (SDL_IntersectRect(&_actor1->GetRect(), &_actor2->GetRect(), &result))
	{
		VECTOR2 pos1 = ((IDynamicActor*)_actor1)->GetPosition();
		VECTOR2 pos2 = ((IDynamicActor*)_actor2)->GetPosition();

		ICharacter* player = ((ICharacter*)_actor1->GetOwner());
		ICharacter* enemy = ((ICharacter*)_actor2->GetOwner());

		Bool playerAttacking = player->IsAttacking();
		Bool enemyAttacking = enemy->IsAttacking();

		if (enemyAttacking && playerAttacking || enemyAttacking)
		{
			_actor1->SetPECollided(true);
		}
		else if (!enemyAttacking && playerAttacking)
		{
			_actor2->SetPECollided(true);
		}
		else // Neither attacking, enemy face player and attack
		{
			if (pos1.x < pos2.x) // player to the left
			{
				enemy->TriggerAttack(true);
			}
			else // player to the right
			{
				enemy->TriggerAttack(false);
			}
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

void Physics::EnemyStaticCollision(IActor* _actor1, IActor* _actor2)
{
	// actor1 is always dynamic
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

void Physics::PlayerWaterCollision(IActor* _actor1, IActor* _actor2)
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
			//if (pos2.y < pos.y) // below, should never hit this one
			//{
			//	//pos.y = pos2.y + rect2.h * 0.5f + rect1.h * 0.5f;
			//	_actor1->SetPWCollided(true);
			//}
			if (pos2.y > pos.y) // above
			{
				if (pos.y > pos2.y - rect2.h)
				{
					_actor1->SetPWCollided(true);
				}
			}
		}
	}
}

void Physics::PlayerCoinCollision(IActor* _actor1, IActor* _actor2)
{
	// actor1 is always the player
	// actor2 is always the coin physics object
	SDL_Rect result;
	SDL_Rect rect1 = _actor1->GetRect();
	SDL_Rect rect2 = _actor2->GetRect();
	if (SDL_IntersectRect(&rect1, &rect2, &result))
	{
		VECTOR2 pos = _actor1->GetPosition();
		VECTOR2 pos2 = _actor2->GetPosition();

		_actor2->SetPCCollided(true);
	}
}

Int32 Physics::ToggleRenderDebug(lua_State* L)
{
	m_renderDebug = !m_renderDebug;
	return 0;
}