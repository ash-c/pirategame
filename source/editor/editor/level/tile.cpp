
// Library Includes

// Local Includes
#include "tile.h"

CTile::CTile() 
	: m_actor(0)
	, m_sprite(0)
	, m_screenW(0)
	, m_screenH(0)
	, m_type(TYPE_ALONE)
{
	SDL_memset(&m_clips, 0, sizeof(Int32) * 4);
	m_linked.clear();
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

	SetType(_type);

	m_actor = Physics::CreateStaticActor(m_pos, VECTOR2(static_cast<Float32>(TILE_WIDTH),static_cast<Float32>(TILE_HEIGHT)));
	assert(m_actor);

	for (UInt16 i = 0; i < 8; ++i) { m_around[i] = _pos; }

	m_around[0].x -= TILE_WIDTH; // left
	m_around[1].x -= TILE_WIDTH; // top left
	m_around[1].y -= TILE_HEIGHT; 
	m_around[2].y -= TILE_HEIGHT; // top
	m_around[3].x += TILE_WIDTH; // top right
	m_around[3].y -= TILE_HEIGHT;
	m_around[4].x += TILE_WIDTH; // right
	m_around[5].x += TILE_WIDTH; // bottom right
	m_around[5].y += TILE_HEIGHT;
	m_around[6].y += TILE_HEIGHT; // bottom
	m_around[7].x -= TILE_WIDTH; // bottom left
	m_around[7].y += TILE_HEIGHT; 
	
	m_screenW = Renderer::activeRenderer->GetWidth();
	m_screenH = Renderer::activeRenderer->GetHeight();

	return true;
}

Bool CTile::ShutDown()
{
	PY_SAFE_RELEASE(m_sprite);

	for (UInt16 i = 0; i < m_linked.size(); ++i)
	{
		m_linked[i]->RemoveLink(this);
		m_linked[i]->Release();
		m_linked[i] = 0;
	}

	m_linked.clear();
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

void CTile::AddLinked(CTile* _link)
{
	if (0 != _link)
	{
		_link->AddRef();
		m_linked.push_back(_link);
	}

	UpdateType();

	for (UInt16 i = 0; i < m_linked.size(); ++i)
	{
		m_linked[i]->UpdateType();
	}
}

void CTile::UpdateType()
{
	Bool around[8];
	SDL_memset(around, 0, sizeof(Bool) * 8); // ensure all false

	for (UInt16 i = 0; i < m_linked.size(); ++i)
	{
		if (0 != m_linked[i])
		{
			VECTOR2 pos = m_linked[i]->GetPos();
			// left
			if (m_around[0].x == pos.x && m_around[0].y == pos.y)
			{
				around[0] = true;
			}
			// top left
			if (m_around[1].x == pos.x && m_around[1].y == pos.y)
			{
				around[1] = true;
			}
			// top
			if (m_around[2].x == pos.x && m_around[2].y == pos.y)
			{
				around[2] = true;
			}
			// top right
			if (m_around[3].x == pos.x && m_around[3].y == pos.y)
			{
				around[3] = true;
			}
			// right
			if (m_around[4].x == pos.x && m_around[4].y == pos.y)
			{
				around[4] = true;
			}
			// bottom right
			if (m_around[5].x == pos.x && m_around[5].y == pos.y)
			{
				around[5] = true;
			}
			// bottom
			if (m_around[6].x == pos.x && m_around[6].y == pos.y)
			{
				around[6] = true;
			}
			// bottom left
			if (m_around[7].x == pos.x && m_around[7].y == pos.y)
			{
				around[7] = true;
			}
		}
	}
	
	if (around[0] && around[4] )
	{
		SetType(TYPE_MID);
	}
	else if (around[0])
	{
		SetType(TYPE_RIGHT);
	}
	else if (around[4])
	{
		SetType(TYPE_LEFT);
	}
	else if (around[2] && around[6])
	{
		SetType(TYPE_MID_VERT);
	}
	else if (around[6])
	{
		SetType(TYPE_TOP_VERT);
	}
	else if (around[2])
	{
		SetType(TYPE_BOT_VERT);
	}
	
	if (around[0] && around[2] && around[4] && around[6])
	{
		SetType(TYPE_MID_MID);
	}
	else if (around[2] && around[3] && around[4] && around[6])
	{
		SetType(TYPE_MID_LEFT);
	}
	else if (around[2] && around[4] && around[5] && around[6])
	{
		SetType(TYPE_MID_LEFT);
	}
	else if (around[0] && around[1] && around[2] && around[6])
	{
		SetType(TYPE_MID_RIGHT);
	}
	else if (around[0] && around[2] && around[6] && around[7])
	{
		SetType(TYPE_MID_RIGHT);
	}	
	else if (around[0] && around[2] && around[4])
	{
		SetType(TYPE_BOT_MID);
	}
	else if (around[0] && around[4] && around[6])
	{
		SetType(TYPE_TOP_MID);
	}
	else if (around[2] && around[4] && around[6])
	{
		SetType(TYPE_MID_LEFT);
	}
	else if (around[0] && around[2] && around[6])
	{
		SetType(TYPE_MID_RIGHT);
	}
	else if (around[2] && around[4])
	{
		SetType(TYPE_BOT_LEFT);
	}
	else if (around[4] && around[6])
	{
		SetType(TYPE_TOP_LEFT);
	}
	else if (around[0] && around[6])
	{
		SetType(TYPE_TOP_RIGHT);
	}
	else if (around[0] && around[2])
	{
		SetType(TYPE_BOT_RIGHT);
	}

	if (!around[0] && !around[1] && !around[2] && !around[3] && !around[4] && !around[5] && !around[6] && !around[7])
	{
		SetType(TYPE_ALONE);
	}
}

void CTile::RemoveLink(CTile* _link)
{
	for (UInt32 i = 0; i < m_linked.size(); ++i)
	{
		if (m_linked[i] == _link)
		{
			m_linked[i]->Release();
			m_linked.erase(m_linked.begin() + i);
			this->UpdateType();
			break;
		}
	}
}