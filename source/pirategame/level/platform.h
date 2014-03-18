#pragma once

#ifndef __PIRATEGAME_LEVEL_PLATFORM_H__
#define __PIRATEGAME_LEVEL_PLATFORM_H__

// Library Includes

// Local Includes
#include "../../core/core.h"

using namespace Papyrus;

class CPlatform
{
	// Member Functions
public:
	CPlatform();
	~CPlatform();

	Bool		Initialise(FileParser::IParser* _setup, Int8* _tileset, Int32 _number, Int32 _platNum);
	Bool		ShutDown();

	void		Process(Float32 _delta);
	void		Render(VECTOR2 _camPos);

	Bool		IsActive() { return m_actor->IsActive(); }

	// Member Variables
protected:
	Sprite::ISprite**		m_sprites;
	Physics::IDynamicActor*	m_actor;

	SDL_Rect*				m_clips;
	VECTOR2*				m_positions;

	VECTOR2					m_platPosition;
	Int32					m_numSprites;
	Int32					m_screenW;
	Int32					m_screenH;
	Bool					m_moveLeft;
};

#endif // __PIRATEGAME_LEVEL_PLATFORM_H__