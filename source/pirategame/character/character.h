#pragma once

#ifndef __PIRATEGAME_CHARACTER_H__
#define __PIRATEGAME_CHARACTER_H__

// Library Includes

// Local Includes
#include "../core/core.h"

using namespace Papyrus;

class ICharacter
{
	// Member Functions
public:
	ICharacter() {}
	virtual ~ICharacter() {}

	virtual Bool		Initialise(Int8* _spriteSheet, Int8* _spriteInfo) = 0;
	virtual Bool		ShutDown() = 0;

	virtual void		Process(Float32 _delta) = 0;
	virtual void		Render() = 0;

	// Member Variables
protected:
	static const Int32	SPEED = 400;

	Sprite::ISprite*	m_sprite;
	VECTOR2				m_pos;
};

#endif // __PIRATEGAME_CHARACTER_H__