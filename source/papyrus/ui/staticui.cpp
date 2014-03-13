
// Library Includes

// Local Includes
#include "staticui.h"

using namespace Papyrus;

UI::CStaticUI::CStaticUI()
{
}

UI::CStaticUI::~CStaticUI()
{
}

Bool UI::CStaticUI::Initialise(Int8* _path)
{
	m_objects = new CUIObject*[m_numObjects];
	assert(m_objects);

	return true;
}

Bool UI::CStaticUI::ShutDown()
{
	VALIDATE(IUIInterface::ShutDown());
	return true;
}

void UI::CStaticUI::Process(Float32 _delta)
{
	for (UInt16 i = 0; i < m_numObjects; ++i)
	{
		if (m_objects[i]->IsActive())
		{
			m_objects[i]->Process(_delta);
		}
	}
}

void UI::CStaticUI::Render()
{
}