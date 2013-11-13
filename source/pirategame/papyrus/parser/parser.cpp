
// Library Includes

// Local Includes
#include "iparser.h"
#include "ini/iniparser.h"
#include "json/jsonparser.h"
#include "xml/xmlparser.h"

// This Include
#include "parser.h"

using namespace Papyrus;

FileParser::IParser**	FileParser::parserArray = 0;
Float32*				FileParser::timerArray = 0;
UInt16					FileParser::maxNumParsers = 10;
UInt16					FileParser::timeDelay = 30;

Bool FileParser::Initialise()
{
	assert(maxNumParsers > 0);

	timerArray = new Float32[maxNumParsers];
	parserArray = new IParser*[maxNumParsers];
	assert(parserArray && timerArray && "FileParser initialisation failed.");

	memset(parserArray, 0, sizeof(IParser*) * maxNumParsers);
	memset(timerArray, 0, sizeof(Float32) * maxNumParsers);

	return true;
}

void FileParser::Process(Float32 _fDelta)
{
	for (Int16 i = 0; i < maxNumParsers; ++i)
	{
		if (timerArray[i] > 0.0f)
		{
			timerArray[i] -= _fDelta;
			if (timerArray[i] < 0.0f)
			{
				parserArray[i] = FlushFile(parserArray[i]);
			}
		}
	}
}

Bool FileParser::ShutDown()
{
	/*for (Int16 i = 0; i < maxNumParsers; ++i)
	{
		if (0 != parserArray[i])
		{
			parserArray[i]->Release();
			parserArray[i]->ShutDown();
			CLEANDELETE(parserArray[i]);
		}
	}*/
	PY_CLEANARRAY(parserArray, maxNumParsers);

	CLEANARRAY(parserArray);
	CLEANARRAY(timerArray);

	return true;
}

FileParser::IParser* FileParser::CreateParser(Int8* _path)
{
	assert(parserArray && timerArray && "Can't create, arrays missing.");
	assert(0 != _path && "Missing filepath to create.");

	// First check that file has not already been loaded.
	for (Int32 i = 0; i < maxNumParsers; ++i)
	{
		if (0 != parserArray[i])
		{
			if (parserArray[i]->CompareFilePath(_path))
			{
				return parserArray[i]; // Return the parser that already exists for that file.
			}
		}
	}

	// Create parser.
	IParser* newParser = 0;

	// Get type of parser to create from extension.
	// Get position of last period.
	const Int8* dot = strrchr(_path, '.');
	Int32 dotPos = dot - _path;
	
	// Set pointer to the file extension using position of the last period.
	Int8* type = &_path[dotPos + 1];
	Int32 length = strlen(type);
	Int8* lower = new Int8[length + 1];

	// Make sure it is lower case for comparison purposes.
	for (Int32 i = 0; i < length; ++i)
	{
		lower[i] = tolower(type[i]);
	}
	lower[length] = 0;

	// Determine what type of parser to create.
	if (!strcmp(lower, "ini"))
	{
		CREATEPOINTER(newParser, CIniparser);
	}
	else if (!strcmp(lower, "json"))
	{
		CREATEPOINTER(newParser, CJSONParser);
	}
	else if (!strcmp(lower, "xml"))
	{
		CREATEPOINTER(newParser, CXMLParser);
	}
	CLEANARRAY(lower);

	for (Int16 i = 0; i < maxNumParsers; ++i)
	{
		if (0 == parserArray[i])
		{
			parserArray[i] = newParser;
			timerArray[i] = timeDelay;
			VALIDATE(newParser->Initialise(_path, true));
			return newParser;
		}
	}
	// No free space!
	CLEANDELETE(newParser);
	assert(0 && "Set maxNumParsers to be a larger value.");
	return 0;
}

FileParser::IParser* FileParser::LoadFile(Int8* _filePath)
{
	assert(parserArray && timerArray && "Can't load, arrays missing.");
	assert(0 != _filePath && "Missing filepath to load.");

	// First check that file has not already been loaded.
	for (Int32 i = 0; i < maxNumParsers; ++i)
	{
		if (0 != parserArray[i])
		{
			if (parserArray[i]->CompareFilePath(_filePath))
			{
				return parserArray[i]; // Return the parser that already exists for that file.
			}
		}
	}

	FileParser::IParser* temp = CreateParser(_filePath);
	if (temp)
	{
		temp->Load(_filePath);
		return temp;
	}
	return 0;
}

FileParser::IParser* FileParser::FlushFile(IParser* _parser, Bool _delete)
{
	if (0 != parserArray)
	{
		assert(0 != _parser);
		for (Int16 i = 0; i < maxNumParsers; ++i)
		{
			if (_parser == parserArray[i])
			{
				if (_delete)
				{
					PY_DELETE_RELEASE(parserArray[i]);
				}
				else
				{
					parserArray[i] = 0;
				}
				return 0;
			}
		}
	}

	return _parser;
}