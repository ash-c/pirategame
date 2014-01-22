
// Library Includes

// Local Includes
#include "tile.h"

CTile::CTile() 
	: m_actor(0)
	, m_sprite(0)
	, m_screenW(0)
	, m_screenH(0)
{
	SDL_memset(&m_clips, 0, sizeof(Int32) * 4);
}

CTile::~CTile() 
{
}

Bool CTile::Initialise(Int8* _spritesheet, VECTOR2 _pos, ETileType _type)
{
	m_sprite = Sprite::CreateSprite(_spritesheet, 0, false);
	assert(m_sprite);
	m_sprite->AddRef();
	m_sprite->SetScale(TILE_WIDTH, TILE_HEIGHT);
	m_clips.w = TILE_WIDTH;
	m_clips.h = TILE_HEIGHT;
	m_pos = _pos;
	m_type = _type;

	SetType(m_type);

	m_actor = Physics::CreateStaticActor(m_pos, VECTOR2(static_cast<Float32>(TILE_WIDTH),static_cast<Float32>(TILE_HEIGHT)));
	assert(m_actor);
	
	m_screenW = Renderer::activeRenderer->GetWidth();
	m_screenH = Renderer::activeRenderer->GetHeight();

	return true;
}

Bool CTile::ShutDown()
{
	PY_SAFE_RELEASE(m_sprite);
	return true;
}

void CTile::Render(VECTOR2 _camPos)
{
	if ((m_pos.x + _camPos.x) > -TILE_WIDTH && (m_pos.y + _camPos.y) > -TILE_HEIGHT &&
		(m_pos.x + _camPos.x) < (m_screenW + TILE_WIDTH) && (m_pos.y + _camPos.y) < (m_screenH + TILE_HEIGHT))
	{
		m_actor->SetActive(true);
		m_sprite->SetClip(&m_clips);
		m_sprite->SetPosition(static_cast<Int32>(m_pos.x + _camPos.x), static_cast<Int32>(m_pos.y + _camPos.y));
		m_sprite->Render();
	}
	else if (m_actor->IsActive())
	{
		m_actor->SetActive(false);
	}
}

void CTile::SetType(ETileType _type)
{
	m_type = _type;
	switch(m_type)
	{
	case TYPE_TOP_LEFT:
		m_clips.x = 0;
		m_clips.y = 0;
		break;
	case TYPE_TOP_MID:
		m_clips.x = TILE_WIDTH;
		m_clips.y = 0;
		break;
	case TYPE_TOP_RIGHT:
		m_clips.x = TILE_WIDTH_DOUBLE;
		m_clips.y = 0;
		break;
	case TYPE_MID_LEFT:
		m_clips.x = 0;
		m_clips.y = TILE_HEIGHT;
		break;
	case TYPE_MID_MID:
		m_clips.x = TILE_WIDTH;
		m_clips.y = TILE_HEIGHT;
		break;
	case TYPE_MID_RIGHT:
		m_clips.x = TILE_WIDTH_DOUBLE;
		m_clips.y = TILE_HEIGHT;
		break;
	case TYPE_BOT_LEFT:
		m_clips.x = 0;
		m_clips.y = TILE_HEIGHT_DOUBLE;
		break;
	case TYPE_BOT_MID:
		m_clips.x = TILE_WIDTH;
		m_clips.y = TILE_HEIGHT_DOUBLE;
		break;
	case TYPE_BOT_RIGHT:
		m_clips.x = TILE_WIDTH_DOUBLE;
		m_clips.y = TILE_HEIGHT_DOUBLE;
		break;
	case TYPE_LEFT:
		m_clips.x = 0;
		m_clips.y = TILE_HEIGHT_TRIPLE;
		break;
	case TYPE_MID:
		m_clips.x = TILE_WIDTH;
		m_clips.y = TILE_HEIGHT_TRIPLE;
		break;
	case TYPE_RIGHT:
		m_clips.x = TILE_WIDTH_DOUBLE;
		m_clips.y = TILE_HEIGHT_TRIPLE;
		break;
	case TYPE_TOP_VERT:
		m_clips.x = 0;
		m_clips.y = TILE_HEIGHT_QUAD;
		break;
	case TYPE_MID_VERT:
		m_clips.x = TILE_WIDTH;
		m_clips.y = TILE_HEIGHT_QUAD;
		break;
	case TYPE_BOT_VERT:
		m_clips.x = TILE_WIDTH_DOUBLE;
		m_clips.y = TILE_HEIGHT_QUAD;
		break;
	case TYPE_ALONE:
		m_clips.x = 0;
		m_clips.y = TILE_HEIGHT_QUAD + TILE_HEIGHT;
		break;
	default:
		break;
	}
}

VECTOR2 CTile::GetPos() 
{ 
	return m_pos; 
}

ETileType CTile::GetType()
{
	return m_type;
}