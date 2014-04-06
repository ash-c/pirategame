
// Library Includes

// Local Includes
#include "controllable.h"
#include "../logging/logger.h"

using namespace Papyrus::Physics;

CControllable::CControllable()
	: m_owner(0)
{
}

CControllable::~CControllable()
{
}

Bool CControllable::Initialise(VECTOR2 _maxVel, VECTOR2 _maxAcc, VECTOR2 _pos, VECTOR2 _scale, Float32 _mass, EType _type)
{
	if (_type == Physics::EType::TYPE_PLAYER)
	{
		Logger::TrackValue(&m_currState.pos, "position");
		Logger::TrackValue(&m_currState.vel, "velocity");
		Logger::TrackValue(&m_currState.acc, "acceleration");
	}

	m_currState.pos = _pos;
	m_currState.preP = m_currState.pos;
	m_mass = _mass;
	m_maxState.vel = _maxVel;
	m_maxState.acc = _maxAcc;
	m_type = _type;

	m_bounds.topLX = _pos.x - _scale.x * 0.5f;
	m_bounds.topLY = _pos.y - _scale.y * 0.5f;
	m_bounds.botRX = _pos.x + _scale.x * 0.5f;
	m_bounds.botRY = _pos.y + _scale.y * 0.5f;

	m_bounds.rect.x = static_cast<Int32>(m_bounds.topLX);
	m_bounds.rect.y = static_cast<Int32>(m_bounds.topLY);
	m_bounds.rect.w = static_cast<Int32>(_scale.x);
	m_bounds.rect.h = static_cast<Int32>(_scale.y);

	return true;
}

Bool CControllable::ShutDown()
{
	if (m_type == Physics::EType::TYPE_PLAYER)
	{
		Logger::StopTracking("position");
		Logger::StopTracking("velocity");
		Logger::StopTracking("acceleration");
	}

	return true;
}

void CControllable::Process(Float32 _delta)
{
	IDynamicActor::Process(_delta);
}

void CControllable::SetOwner(void* _owner)
{
	m_owner = _owner;
}

void* CControllable::GetOwner()
{
	return m_owner;
}
