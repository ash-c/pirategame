function load(s)
	LoadLevel(s)
	return 'Loaded level '..s..''
end

function editor(s)
	LoadFile(s)
	return 'Loaded file '..s..''
end