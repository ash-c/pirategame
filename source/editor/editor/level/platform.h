#pragma once

#ifndef __PIRATEGAME_LEVEL_PLATFORM_H__
#define __PIRATEGAME_LEVEL_PLATFORM_H__

// Library Includes
#include <vector>

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

	Bool		Save(FileParser::IParser* _save, Int32 _platNum);

	Bool		CheckPosition(VECTOR2 _pos);
	Bool		AddPosition(VECTOR2 _pos, Int8* _tileset);
	Bool		RemovePosition(VECTOR2 _pos);
	UInt32		GetNumPositions() const;

private:
	void		CheckType(SDL_Rect* _clip, UInt32 _type);

	// Member Variables
protected:
	std::vector<Sprite::ISprite*> m_sprites;
	Physics::IDynamicActor*	m_actor;

	std::vector<SDL_Rect>	m_clips;
	std::vector<VECTOR2>	m_positions;

	VECTOR2					m_platPosition;
	VECTOR2					m_scale;
	Int32					m_numSprites;
	Int32					m_screenW;
	Int32					m_screenH;
	UInt32					m_newType;
	Bool					m_moveLeft;
};

#endif // __PIRATEGAME_LEVEL_PLATFORM_H__