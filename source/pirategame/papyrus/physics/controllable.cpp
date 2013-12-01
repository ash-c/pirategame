
// Library Includes

// Local Includes
#include "controllable.h"
#include "../logging/logger.h"

using namespace Papyrus::Physics;

CControllable::CControllable()
{
}

CControllable::~CControllable()
{
}

Bool CControllable::Initialise(VECTOR2 _maxVel, VECTOR2 _pos, Float32 _mass)
{
	Logger::TrackValue(&m_currState.pos, "position");
	Logger::TrackValue(&m_currState.vel, "velocity");

	m_currState.pos = _pos;
	m_mass = _mass;
	m_maxVel = _maxVel;

	return true;
}

Bool CControllable::ShutDown()
{
	return true;
}

void CControllable::Process(Float32 _delta)
{
	IDynamicActor::Process(_delta);
}