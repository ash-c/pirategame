function togglescreen()
	ToggleScreenDebug()
	return 'Screen debugging toggled'
end

function toggleconsole()
	ToggleConsole()
	return 'Console toggled'
end

function test(i,s,b)
	TestFunction(i,s,b)
	return 'Test function run'
end

function exit()
	QuitGame()
	return 'Game exiting...'
end