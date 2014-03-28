#pragma once

#ifndef __PIRATEGAME_PARALLAX_H__
#define __PIRATEGAME_PARALLAX_H__

// Library Includes
#include <vector>

// Local Includes
#include "../../papyrus/core/core.h"

using namespace Papyrus;

class CParallax
{
	// Member Functions
public:
	CParallax();
	~CParallax();

	Bool		Initialise(FileParser::IParser* _setup, Int8* _num, Bool _animate = false);
	Bool		ShutDown();

	void		Process(Float32 _delta);
	void		Render(VECTOR2 _cameraPos);

	// Member Variables
protected:
	std::vector<Sprite::ISprite*>	m_sprites;
	std::vector<VECTOR2>			m_positions;
	std::vector<VECTOR2>			m_speed;
	std::vector<Bool>				m_move;

	Float32		m_scale;

	Bool		m_animated;
};
#endif // __PIRATEGAME_PARALLAX_H__