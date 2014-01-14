
// Library Includes

// Local Includes
#include "leveledit.h"
#include "level\level.h"

CLevelEdit::CLevelEdit()
	: m_level(0)
{
}

CLevelEdit::~CLevelEdit()
{
}

Bool CLevelEdit::Initialise()
{
	CREATEPOINTER(m_level, CLevel);
	assert(m_level);
	VALIDATE(m_level->Initialise("data/levels/newlevel.json"));
	m_level->AddRef();

	return true;
}

Bool CLevelEdit::ShutDown()
{
	PY_DELETE_RELEASE(m_level);
	return true;
}

void CLevelEdit::Process(Float32 _delta)
{
}

void CLevelEdit::Render()
{
	if (0 != m_level) m_level->Render();
}

void CLevelEdit::Notify(SDL_Event* _e)
{
}