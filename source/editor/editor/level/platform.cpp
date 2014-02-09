
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

	m_sprites = new Sprite::ISprite*[m_numSprites];
	SDL_memset(m_sprites, 0 , sizeof(Sprite::ISprite*) * m_numSprites);
	m_clips = new SDL_Rect[m_numSprites];
	SDL_memset(m_clips, 0 , sizeof(SDL_Rect) * m_numSprites);
	m_positions = new VECTOR2[m_numSprites];
	SDL_memset(m_positions, 0 , sizeof(VECTOR2) * m_numSprites);

	for (Int16 i = 0; i < m_numSprites; ++i)
	{
		m_sprites[i] = Sprite::CreateSprite(_tileset, 0, false);
		assert(m_sprites[i]);
		m_sprites[i]->AddRef();

		SDL_snprintf(text, MAX_BUFFER, "%iplatform-%ipos", _platNum, i + 1);
		VALIDATE(_setup->GetValue(text, m_positions[i]));
		SDL_snprintf(text, MAX_BUFFER, "%iplatform-%itype", _platNum, i + 1);
		VALIDATE(_setup->GetValue(text, type));

		m_clips[i].w = 50;
		m_clips[i].h = 50;

		switch (type)
		{
		case TYPE_LEFT:
			m_clips[i].x = 0;
			m_clips[i].y = 150;
			break;
		case TYPE_MID:
			m_clips[i].x = 50;
			m_clips[i].y = 150;
			break;
		case TYPE_RIGHT:
			m_clips[i].x = 100;
			m_clips[i].y = 150;
			break;
		default:
			m_clips[i].x = 0;
			m_clips[i].y = 0;
			break;
		}
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
	if (m_actor->IsCollided())
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
	for (Int16 i = 0; i < m_numSprites; ++i)
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
	for (Int16 i = 0; i < m_numSprites; ++i)
	{
		m_sprites[i]->SetClip(&m_clips[i]);
		m_sprites[i]->SetPosition(static_cast<Int32>(m_positions[i].x), static_cast<Int32>(m_positions[i].y));
		m_sprites[i]->Render();
	}
}