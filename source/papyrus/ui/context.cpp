
// Library Includes

// Local Includes
#include "context.h"
#include "../logging/logger.h"
#include "../parser/parser.h"
#include "objects/uibutton.h"

#include "ui.h"

using namespace Papyrus;

UI::CContextMenu::CContextMenu()
	: m_background(0)
{
}

UI::CContextMenu::~CContextMenu()
{
}

Bool UI::CContextMenu::Initialise(Int8* _path)
{
	assert(0 != _path);
	Int16 len = SDL_strlen(_path) + 1;
	m_filePath = new Int8[len];
	SDL_strlcpy(m_filePath, _path, len);

	FileParser::IParser* setup = FileParser::LoadFile(_path);
	assert(setup);
	setup->AddRef();

	UInt32 numButtons = 0, numStatic = 0;
	Int8* luaFile = 0;
	VALIDATE(setup->GetValue("buttons", numButtons, "info"));
	VALIDATE(setup->GetValue("static", numStatic, "info"));
	VALIDATE(setup->GetValue("lua", &luaFile, "info"));
	VALIDATE(setup->GetValue("width", m_width, "info"));

	// Calculate actual height from number of objects in menu.
	m_height = 0;

	// Will be other types of objects.
	m_numObjects = numButtons + numStatic;

	m_objects = new CUIObject*[m_numObjects];
	assert(m_objects);
	SDL_memset(m_objects, 0, sizeof(CUIObject*) * m_numObjects);
	
	UInt32 h = 0;
	VECTOR2 pos;
	Int8* spritePath = 0;
	Int8* luaFunc = 0;
	Int8 buffer[MAX_BUFFER];
	UInt16 count = 0;
	
	// Make static images.
	UInt16 k;
	for (k = 0; k < numStatic; ++k)
	{
		++count;
	}

	// Make buttons.
	for (k = 0; k < numButtons; ++k)
	{
		CREATEPOINTER(m_objects[count], CUIButton);
		assert(m_objects[count]);

		SDL_snprintf(buffer, MAX_BUFFER, "button%i", k + 1);
		setup->GetValue("h", h, buffer);
		setup->GetValue("sprite", &spritePath, buffer);
		setup->GetValue("lua", &luaFunc, buffer);

		m_height += h;
		pos.y += h;

		m_objects[count]->Initialise(luaFile, luaFunc, spritePath, pos, m_width, h);
		m_objects[count]->AddRef();
		CLEANARRAY(spritePath);
		CLEANARRAY(luaFunc);
		++count;	
	}
	CLEANARRAY(luaFile);

	VALIDATE(setup->GetValue("background", &luaFile, "info"));

	m_background = Sprite::CreateSprite(luaFile, 0, false);
	assert(m_background);
	m_background->AddRef();
	m_background->SetScale(static_cast<Int32>(m_width * UI::wScale), m_height);

	CLEANARRAY(luaFile);

	setup->Release();

	m_active = false;
	return true;
}

Bool UI::CContextMenu::ShutDown()
{
	PY_SAFE_RELEASE(m_background);
	VALIDATE(IUIInterface::ShutDown());
	return true;
}

void UI::CContextMenu::Render()
{
	if (m_active)
	{
		if (0 != m_background) 
		{
			m_background->SetPosition(static_cast<Int32>(m_pos.x), static_cast<Int32>(m_pos.y + m_height * 0.5f));
			m_background->Render();
		}

		for (UInt16 i = 0; i < m_numObjects; ++i)
		{
			if (m_objects[i]->IsActive())
			{
				m_objects[i]->SetPosition(VECTOR2(m_pos.x, m_pos.y + m_objects[i]->GetHeight() * i));
				m_objects[i]->Render();
			}
		}
	}
}

void UI::CContextMenu::Show(VECTOR2* _pos)
{
	if (0 != _pos)
	{
		m_pos = *_pos;
		m_pos.x += m_width * 0.5f;
		//m_pos.y += m_height * 0.5f;
	}

	IUIInterface::Show();
}