function setposition(id,x,y)
	SetSpritePosition(id,x,y)
	return 'Sprite id: '..id..' position set to ('..x..','..y..')'
end

function setscale(id,w,h)
	SetSpriteScale(id,w,h)
	return 'Sprite id: '..id..' scale set to width: '..w..', height: '..h..''
end