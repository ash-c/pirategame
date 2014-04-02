#pragma once

#ifndef __PIRATEGAME_LEVEL_COIN_H__
#define __PIRATEGAME_LEVEL_COIN_H__

// Library Includes

// Local Includes
#include "../../papyrus/core/core.h"

using namespace Papyrus;

class CCoin
{
	// Member Functions
public:
	CCoin();
	~CCoin();

	Bool			Initialise(VECTOR2 _pos);
	Bool			ShutDown();

	void			Process(Float32 _delta);
	void			Render(VECTOR2 _camPos);

	Bool			IsActive() { return m_actor->IsActive(); }

	VECTOR2			GetPos() { return m_pos; }

	// Member Functions
protected:
	Sprite::ISprite*	m_sprite;
	Physics::IActor*	m_actor;

	VECTOR2				m_pos;

	Int32				m_screenW;
	Int32				m_screenH;
};

#endif // __PIRATEGAME_LEVEL_COIN_H__