function setplacetool()
	ChangeTool(1)
	return 'tool changed to place tiles'
end

function setremovetool()
	ChangeTool(0)
	return 'tool changed to remove'
end

function setbasicenemytool()
	ChangeTool(3)
	return 'tool changed to basic enemy'
end

function playerstarttool()
	ChangeTool(4)
	return 'tool changed to player start'
end

function platformstool()
	ChangeTool(2)
	return 'tool changed to moving platforms'
end

function setwatertool()
	ChangeTool(5)
	return 'tool changed to place water'
end