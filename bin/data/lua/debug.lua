function togglescreen()
	ToggleScreenDebug()
	return 'Screen debugging toggled'
end

function toggleconsole()
	ToggleConsole()
	return 'Console toggled'
end

function togglephysics()
	ToggleRenderDebug()
	return 'Physics debuging toggled'
end

function togglecursor()
	ToggleCursor()
	return 'Cursor toggled'
end

function test(i,s,b)
	TestFunction(i,s,b)
	return 'Test function run'
end

function exit()
	QuitGame()
	return 'Game exiting...'
end