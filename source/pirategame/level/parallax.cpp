
// Library Includes

// Local Includes
#include "parallax.h"
#include "level.h"

CParallax::CParallax()
	: m_animated(false)
{
	m_sprites.clear();
	m_positions.clear();
}

CParallax::~CParallax()
{
}

Bool CParallax::Initialise(FileParser::IParser* _setup, Int8* _num, Bool _animate)
{
	assert(0 != _setup);
	m_animated = _animate;

	Int32 objects = 0;
	VALIDATE(_setup->GetValue("objects", objects, _num));
	VALIDATE(_setup->GetValue("scale", m_scale, _num));
	// ensure the scale is between 0 and 1
	if (1.0f < m_scale) m_scale = 1.0f;
	else if (0.0f > m_scale) m_scale = 0.0f;

	VECTOR2 pos;
	Int8 text[MAX_BUFFER];
	Int8* path = 0;
	Bool anim = false;

	for (Int16 i = 0; i < objects; ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "%i", i + 1);
		VALIDATE(_setup->GetValue(text, &path, _num));
		SDL_snprintf(text, MAX_BUFFER, "%ianim", i + 1);
		VALIDATE(_setup->GetValue(text, anim, _num));
		SDL_snprintf(text, MAX_BUFFER, "%ipos", i + 1);
		VALIDATE(_setup->GetValue(text, pos, _num));

		Sprite::ISprite* temp = Sprite::CreateSprite(path, 0, anim);
		temp->AddRef();
		m_sprites.push_back(temp);
		m_positions.push_back(pos);
		CLEANARRAY(path);

		// Moving background, or stationary
		SDL_snprintf(text, MAX_BUFFER, "%imove", i + 1);
		VECTOR2 speed;
		if (_setup->GetValue(text, anim, _num))
		{
			SDL_snprintf(text, MAX_BUFFER, "%ispeed", i + 1);
			VALIDATE(_setup->GetValue(text, speed, _num));
		}
		else
		{
			m_speed.push_back(speed);
		}
	}

	return true;
}

Bool CParallax::ShutDown()
{
	for (UInt16 i = 0; i < m_sprites.size(); ++i)
	{
		m_sprites[i]->Release();
		Sprite::FlushFile(m_sprites[i]);
		m_sprites[i] = 0;
	}
	m_sprites.clear();
	return true;
}

void CParallax::Process(Float32 _delta)
{
	for (UInt16 i = 0; i < m_sprites.size(); ++i)
	{
		m_positions[i].x += m_speed[i].x * _delta;
		m_positions[i].y += m_speed[i].y * _delta;
		VECTOR2 scale = m_sprites[i]->GetScale();

		if (m_positions[i].x < 0.0f - scale.x)
		{
			m_positions[i].x = LEVEL_WIDTH + scale.x;
		}

		if (m_positions[i].x > LEVEL_WIDTH + scale.x)
		{
			m_positions[i].x = 0.0f - scale.x;
		}

		m_sprites[i]->Process(_delta);
	}
}

void CParallax::Render(VECTOR2 _cameraPos)
{
	for (UInt16 i = 0; i < m_sprites.size(); ++i)
	{
		VECTOR2 newPos;
		newPos.x = m_positions[i].x + _cameraPos.x * m_scale;
		m_sprites[i]->SetPosition(static_cast<Int32>(newPos.x), static_cast<Int32>(m_positions[i].y - _cameraPos.y));
		m_sprites[i]->Render();

	}
}