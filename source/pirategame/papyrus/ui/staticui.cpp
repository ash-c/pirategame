
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

void UI::CStaticUI::Render()
{
}