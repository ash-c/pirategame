
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
	, m_moveLeft(true)
{
}

CPlatform::~CPlatform()
{
}

Bool CPlatform::Initialise(FileParser::IParser* _setup, Int8* _tileset, Int32 _number, Int32 _platNum)
{
	m_numSprites = _number;
	_setup->AddRef();

	Int8 text[MAX_BUFFER];
	VECTOR2 scale;
	UInt32 type = 0;

	SDL_snprintf(text, MAX_BUFFER, "%iplatform-pos", _platNum);
	VALIDATE(_setup->GetValue(text, m_platPosition));
	SDL_snprintf(text, MAX_BUFFER, "%iplatform-scale", _platNum);
	VALIDATE(_setup->GetValue(text, scale));

	m_actor = Physics::CreateDynamicActor(VECTOR2(300, 0), VECTOR2(0,0), m_platPosition, scale, 100.0f, Physics::EType::TYPE_PLATFORM);
	assert(m_actor);
	m_actor->AddRef();
	m_actor->SetVelocity(VECTOR2(-250,0));
	m_actor->SetActive(true);

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
		clip.w = temp->GetScale().x;
		clip.h = temp->GetScale().y;
		CheckType(&clip, type);
		m_clips.push_back(clip);
	}

	_setup->Release();
	return true;
}

Bool CPlatform::ShutDown()
{
	for (Int16 i = 0; i < m_numSprites; ++i)
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

void CPlatform::Render()
{
	for (UInt16 i = 0; i < m_sprites.size(); ++i)
	{
		m_sprites[i]->SetClip(&m_clips[i]);
		m_sprites[i]->SetPosition(static_cast<Int32>(m_positions[i].x), static_cast<Int32>(m_positions[i].y));
		m_sprites[i]->Render();
	}
}

Bool CPlatform::CheckPosition(VECTOR2 _pos)
{
	for (UInt16 i = 0; i < m_positions.size(); ++i)
	{
		// Is the given position this piece?
		if (m_positions[i] == _pos)
		{
			return false;
		}

		// to the left
		if (m_positions[i].x - m_clips[i].w == _pos.x && m_positions[i].y == _pos.y)
		{
			return true;
		}

		// to the right
		if (m_positions[i].x + m_clips[i].w == _pos.x && m_positions[i].y == _pos.y)
		{
			return true;
		}
	}

	return false;
}

Bool CPlatform::AddPosition(VECTOR2 _pos, Int8* _tileset)
{
	// Add sprite
	Sprite::ISprite* temp = 0;
	temp = Sprite::CreateSprite(_tileset, 0, false);
	assert(temp);
	temp->AddRef();
	m_sprites.push_back(temp);

	// Add position
	m_positions.push_back(_pos);

	// calculate type
	UInt32 type = 0;
	

	// Add clips, based on type
	SDL_Rect clip;
	clip.w = temp->GetScale().x;
	clip.h = temp->GetScale().y;
	CheckType(&clip, type);
	m_clips.push_back(clip);

	return true;
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
	default:
		_clip->x = 0;
		_clip->y = 0;
		break;
	}
}