
// Library Includes

// Local Includes
#include "platform.h"
#include "tile.h"

CPlatform::CPlatform()
	: m_sprites(0)
	, m_actor(0)
	, m_clips(0)
	, m_positions(0)
	, m_numSprites(0)
	, m_screenW(0)
	, m_screenH(0)
	, m_newType(0)
	, m_moveLeft(true)
{
	m_scale.y = TILE_HEIGHT;
}

CPlatform::~CPlatform()
{
}

Bool CPlatform::Initialise(FileParser::IParser* _setup, Int8* _tileset, Int32 _number, Int32 _platNum)
{
	m_numSprites = _number;
	if (0 != _setup)
	{
		_setup->AddRef();

		Int8 text[MAX_BUFFER];
		UInt32 type = 0;

		SDL_snprintf(text, MAX_BUFFER, "%iplatform-pos", _platNum);
		VALIDATE(_setup->GetValue(text, m_platPosition));
		SDL_snprintf(text, MAX_BUFFER, "%iplatform-scale", _platNum);
		VALIDATE(_setup->GetValue(text, m_scale));

		/*m_sprites = new Sprite::ISprite*[m_numSprites];
		SDL_memset(m_sprites, 0 , sizeof(Sprite::ISprite*) * m_numSprites);
		m_clips = new SDL_Rect[m_numSprites];
		SDL_memset(m_clips, 0 , sizeof(SDL_Rect) * m_numSprites);
		m_positions = new VECTOR2[m_numSprites];
		SDL_memset(m_positions, 0 , sizeof(VECTOR2) * m_numSprites);*/

		for (Int16 i = 0; i < m_numSprites; ++i)
		{
			Sprite::ISprite* temp = 0;
			temp = Sprite::CreateSprite(_tileset, 0, false);
			assert(temp);
			temp->AddRef();
			m_sprites.push_back(temp);

			SDL_snprintf(text, MAX_BUFFER, "%iplatform-%ipos", _platNum, i + 1);
			VECTOR2 pos;
			VALIDATE(_setup->GetValue(text, pos));
			m_positions.push_back(pos);
			SDL_snprintf(text, MAX_BUFFER, "%iplatform-%itype", _platNum, i + 1);
			VALIDATE(_setup->GetValue(text, type));

			SDL_Rect clip;
			clip.w = static_cast<Int32>(temp->GetScale().x);
			clip.h = static_cast<Int32>(temp->GetScale().y);
			CheckType(&clip, type);
			m_clips.push_back(clip);
		}

		_setup->Release();
	}

	m_actor = Physics::CreateDynamicActor(VECTOR2(300, 0), VECTOR2(0,0), m_platPosition, m_scale, 100.0f, Physics::EType::TYPE_PLATFORM);
	assert(m_actor);
	m_actor->AddRef();
	m_actor->SetVelocity(VECTOR2(-250,0));
	
	m_screenW = Renderer::activeRenderer->GetWidth();
	m_screenH = Renderer::activeRenderer->GetHeight();

	return true;
}

Bool CPlatform::ShutDown()
{
	for (UInt16 i = 0; i < m_sprites.size(); ++i)
	{
		m_sprites[i]->Release();
	}
	
	m_actor->Release();

	return true;
}

void CPlatform::Process(Float32 _delta)
{
	if (m_actor->IsHCollided())
	{
		if (m_moveLeft)
		{
			m_actor->SetVelocity(VECTOR2(250,0));
			m_moveLeft = false;
		}
		else
		{
			m_actor->SetVelocity(VECTOR2(-250,0));
			m_moveLeft = true;
		}
	}

	VECTOR2 pos = m_actor->GetPosition();
	VECTOR2 diff = pos - m_platPosition;
	for (UInt16 i = 0; i < m_sprites.size(); ++i)
	{
		m_positions[i] += diff;
	}
	m_platPosition = pos;

	if (m_actor->IsPPCollided())
	{
		m_actor->UpdatePlayer(diff);
	}
}

void CPlatform::Render(VECTOR2 _camPos)
{
	if ((m_platPosition.x + _camPos.x) > -m_scale.x && (m_platPosition.y - _camPos.y) > -m_scale.y &&
		(m_platPosition.x + _camPos.x) < (m_screenW + m_scale.x) && (m_platPosition.y - _camPos.y) < (m_screenH + m_scale.y))
	{
		m_actor->SetActive(true);
		for (UInt16 i = 0; i < m_sprites.size(); ++i)
		{
			m_sprites[i]->SetClip(&m_clips[i]);
			m_sprites[i]->SetPosition(static_cast<Int32>(m_positions[i].x + _camPos.x), static_cast<Int32>(m_positions[i].y - _camPos.y));
			m_sprites[i]->Render();
		}
	}
	else if (m_actor->IsActive())
	{
		m_actor->SetActive(false);
	}
}

Bool CPlatform::Save(FileParser::IParser* _save, Int32 _platNum)
{
	_save->AddRef();
	Int8 text[MAX_BUFFER];
	SDL_snprintf(text, MAX_BUFFER, "%iplatform-num", _platNum);

	VALIDATE(_save->AddValue(text, m_sprites.size()));

	SDL_snprintf(text, MAX_BUFFER, "%iplatform-pos", _platNum);
	VALIDATE(_save->AddValue(text, m_platPosition));

	SDL_snprintf(text, MAX_BUFFER, "%iplatform-scale", _platNum);
	VALIDATE(_save->AddValue(text, m_scale));

	for (UInt16 i = 0; i < m_sprites.size(); ++i)
	{
		SDL_snprintf(text, MAX_BUFFER, "%iplatform-%ipos", _platNum, i + 1);
		VALIDATE(_save->AddValue(text, m_positions[i]));

		if (0 == m_clips[i].x)
		{
			if (250 == m_clips[i].y)
			{
				SDL_snprintf(text, MAX_BUFFER, "%iplatform-%itype", _platNum, i + 1);
				VALIDATE(_save->AddValue(text, TYPE_ALONE));
			}
			else 
			{
				SDL_snprintf(text, MAX_BUFFER, "%iplatform-%itype", _platNum, i + 1);
				VALIDATE(_save->AddValue(text, TYPE_LEFT));
			}
		}
		if (50 == m_clips[i].x)
		{
			SDL_snprintf(text, MAX_BUFFER, "%iplatform-%itype", _platNum, i + 1);
			VALIDATE(_save->AddValue(text, TYPE_MID));
		}
		if (100 == m_clips[i].x)
		{
			SDL_snprintf(text, MAX_BUFFER, "%iplatform-%itype", _platNum, i + 1);
			VALIDATE(_save->AddValue(text, TYPE_RIGHT));
		}
	}

	_save->Release();

	return true;
}

Bool CPlatform::CheckPosition(VECTOR2 _pos)
{
	m_newType = TYPE_ALONE;
	Bool left = false;
	Bool right = false;
	Bool top = false;
	Bool bot = false;

	for (UInt16 i = 0; i < m_positions.size(); ++i)
	{
		// Is the given position this piece?
		if (m_positions[i] == _pos)
		{
			return true; // returning true as it is this platform
		}

		// to the left
		if (m_positions[i].x - m_clips[i].w == _pos.x && m_positions[i].y == _pos.y)
		{
			left = true;
		}

		// to the right
		if (m_positions[i].x + m_clips[i].w == _pos.x && m_positions[i].y == _pos.y)
		{
			right = true;
		}

		// above
		if (m_positions[i].x == _pos.x && m_positions[i].y - m_clips[i].h == _pos.y)
		{
			top = true;
		}

		// below
		if (m_positions[i].x == _pos.x && m_positions[i].y + m_clips[i].h == _pos.y)
		{
			bot = true;
		}

		if (top || bot) return false;

		// Set type for new piece
		if (left && right) m_newType = TYPE_MID;
		else if (left && !right) m_newType = TYPE_LEFT;
		else if (!left && right) m_newType = TYPE_RIGHT;	

		if (left || right ) return true;
	}

	// No match, new platform
	return false;
}

Bool CPlatform::AddPosition(VECTOR2 _pos, Int8* _tileset)
{
	// check that position isn't already part of this platform
	for (UInt16 i = 0; i < m_positions.size(); ++i)
	{
		if (_pos == m_positions[i]) return false;
	}

	// Add sprite
	Sprite::ISprite* temp = 0;
	Int8 path[MAX_BUFFER];
	SDL_snprintf(path, MAX_BUFFER, "data/art/tilesets/%s/tiles.png", _tileset);
	temp = Sprite::CreateSprite(path, 0, false);
	assert(temp);
	temp->AddRef();
	temp->SetScale(TILE_WIDTH, TILE_HEIGHT);
	m_sprites.push_back(temp);
	++m_numSprites;

	// Add position
	m_positions.push_back(_pos);

	// Add clips, based on type
	SDL_Rect clip;
	clip.w = TILE_WIDTH;
	clip.h = TILE_HEIGHT;
	CheckType(&clip, m_newType);
	m_clips.push_back(clip);

	// adjust physics scale and position
	m_scale.x = static_cast<Float32>(m_positions.size() * TILE_WIDTH);

	Float32 low = 10000.0f;
	Float32 high = -10000.0f;

	for (UInt16 i = 0; i < m_positions.size(); ++i)
	{
		if (m_positions[i].x < low) low = m_positions[i].x;
		else if (m_positions[i].x >= high) high = m_positions[i].x;
	}

	if (-10000.0f == high || 10000.0f == low)
	{
		m_platPosition.x = _pos.x;
	}
	else
	{
		m_platPosition.x = (high - low) * 0.5f + low;
	}
	m_platPosition.y = _pos.y;

	return true;
}

Bool CPlatform::RemovePosition(VECTOR2 _pos)
{
	for (UInt16 i = 0; i < m_positions.size(); ++i)
	{
		if (_pos == m_positions[i])
		{
			m_positions.erase(m_positions.begin() + i);
			m_clips.erase(m_clips.begin() + i);
			PY_SAFE_RELEASE(m_sprites[i]);
			m_sprites.erase(m_sprites.begin() + i);
			--m_numSprites;
			return true;
		}
	}

	return false;
}

UInt32 CPlatform::GetNumPositions() const
{
	return m_positions.size();
}

//
// PRIVATE FUNCTIONS
//
void CPlatform::CheckType(SDL_Rect* _clip, UInt32 _type)
{
	switch (_type)
	{
	case TYPE_LEFT:
		_clip->x = 0;
		_clip->y = 150;
		break;
	case TYPE_MID:
		_clip->x = 50;
		_clip->y = 150;
		break;
	case TYPE_RIGHT:
		_clip->x = 100;
		_clip->y = 150;
		break;
	case TYPE_ALONE:
		_clip->x = 0;
		_clip->y = 250;
		break;
	default:
		_clip->x = 0;
		_clip->y = 0;
		break;
	}
}