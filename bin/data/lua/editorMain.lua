function startLevel()
	StartLevelEditor()
	return 'Level editor started'
end

function startInterface()
	StartInterfaceEditor()
	return 'Interface editor started'
end

function exit()
	Quit()
	return 'Game exited'
end

function togglesc()
	ToggleEscMenu()
	return 'Escape menu toggled'
end

function load()
	LoadFromFile()
	return 'Opened load dialog'
end

function save()
	SaveToFile()
	return 'Opened save dialog'
end

