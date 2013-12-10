
// Library Includes

// Local Includes
#include "static.h"

using namespace Papyrus::Physics;

CStatic::CStatic()
{
}

CStatic::~CStatic()
{
}

Bool CStatic::Initialise(VECTOR2 _pos, VECTOR2 _scale)
{
	m_pos = _pos;
	m_bounds.rect.w = static_cast<Int32>(_scale.x);
	m_bounds.rect.h = static_cast<Int32>(_scale.y);
	m_bounds.topLX = m_pos.x - m_bounds.rect.w * 0.5f;
	m_bounds.topLY = m_pos.y - m_bounds.rect.h * 0.5f;
	m_bounds.botRX = m_pos.x + m_bounds.rect.w * 0.5f;
	m_bounds.botRY = m_pos.y + m_bounds.rect.h * 0.5f;
	m_bounds.rect.x = static_cast<Int32>(m_bounds.topLX);
	m_bounds.rect.y = static_cast<Int32>(m_bounds.topLY);

	return true;
}

Bool CStatic::ShutDown()
{
	return true;
}