
// Library Includes
#include <SDL_image.h>

// Local Includes
#include "sprite.h"
#include "isprite.h"
#include "staticsprite.h"
#include "animsprite.h"

using namespace Papyrus;

Sprite::ISprite**		Sprite::activeSprites = 0;
Renderer::IRenderer*	Sprite::activeRenderer = 0;
UInt16					Sprite::maxNumSprites = 50;

Bool Sprite::Initialise()
{
	assert(maxNumSprites > 0);
	Int32 flags = IMG_INIT_PNG | IMG_INIT_TIF;
	Int32 initResult = IMG_Init(flags);

	if ((initResult & flags) != flags)
	{
		Logger::WriteToFile("IMG_Init failed: %s", IMG_GetError());
		return false;
	}

	activeSprites = new ISprite*[maxNumSprites];
	assert(activeSprites);
	SDL_memset(activeSprites, 0, sizeof(ISprite*) * maxNumSprites);

	activeRenderer = Renderer::activeRenderer;
	activeRenderer->AddRef();

	lua_register(Logger::luaState, "SetSpritePosition", SetSpritePosition);
	lua_register(Logger::luaState, "SetSpriteScale", SetSpriteScale);

	return true;
}

Bool Sprite::ShutDown()
{
	PY_CLEANARRAY(activeSprites, maxNumSprites);

	PY_SAFE_RELEASE(activeRenderer);
	IMG_Quit();
	return true;
}

 Sprite::ISprite* Sprite::CreateSprite(Int8* _spriteSheet, Int8* _setup, Bool _animated)
{
	assert(activeSprites != 0 && "Can't create sprites, array missing");
	assert(_spriteSheet != 0 && "Need a spritesheet path");

	// First check that the sprite has not already been loaded.
	for (Int16 i = 0; i < maxNumSprites; ++i)
	{
		if (0 != activeSprites[i])
		{
			if (activeSprites[i]->CompareFilePath(_spriteSheet))
			{
				return activeSprites[i];
			}
		}
	}

	// Create sprite.
	Sprite::ISprite* sprite = 0;
	if (_animated)
	{
		CREATEPOINTER(sprite, CAnimSprite);
		assert(sprite);
	}
	else
	{
		CREATEPOINTER(sprite, CStaticSprite);
		assert(sprite);
	}

	for (Int16 i = 0; i < maxNumSprites; ++i)
	{
		if (0 == activeSprites[i])
		{
			activeSprites[i] = sprite;
			VALIDATE(sprite->Initialise(_spriteSheet, _setup, i));
			return sprite;
		}
	}
	// No free space!
	PY_DELETE_RELEASE(sprite);
	assert(0 && "Set maxNumSprites to be a larger value - no room with current value");
	return 0;
}

 Sprite::ISprite* Sprite::FlushFile(ISprite* _sprite, Bool _delete)
 {
	if (0 != activeSprites)
	{
		for (Int16 i = 0; i < maxNumSprites; ++i)
		{
			if (_sprite == activeSprites[i])
			{
				if (_delete)
				{
					PY_DELETE_RELEASE(activeSprites[i]);
				}
				else 
				{
					activeSprites[i] = 0;
				}
				return 0;
			}
		}
	}

	 return _sprite;
 }

 Int32 Sprite::SetSpritePosition(lua_State* L)
 {
	 Int16 id = static_cast<Int32>(lua_tonumber(L, 1));
	 Int32 x = static_cast<Int32>(lua_tonumber(L, 2));
	 Int32 y = static_cast<Int32>(lua_tonumber(L, 3));

	 if (0 <= id && id < maxNumSprites)
	 {
		 if (0 != activeSprites[id])
		 {
			activeSprites[id]->SetPosition(x, y);
		 }
		 else
		 {
			 Logger::WriteToConsole("Invalid ID for SetSpritePosition - ID: %i does not exist", id);
		 }
	 }
	 else 
	 {
		 PY_WRITETOCONSOLE("Invalid ID for SetSpritePosition - too low or too high");
	 }

	 return 0;
 }

 Int32 Sprite::SetSpriteScale(lua_State* L)
 {
	 Int16 id = static_cast<Int32>(lua_tonumber(L, 1));
	 Int32 w = static_cast<Int32>(lua_tonumber(L, 2));
	 Int32 h = static_cast<Int32>(lua_tonumber(L, 3));

	 if (0 <= id && id < maxNumSprites)
	 {
		 if (0 != activeSprites[id])
		 {
			 activeSprites[id]->SetScale(w, h);
		 }
		 else
		 {
			 Logger::WriteToConsole("Invalid ID for SetSpriteScale - ID: %i does not exist", id);
		 }
	 }
	 else 
	 {
		 PY_WRITETOCONSOLE("Invalid ID for SetSpriteScale - too low or too high");
	 }

	 return 0;
 }