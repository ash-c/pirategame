
// Library Includes
#include <sstream>
#include <iostream>

using namespace std;

// Local Includes
#include "../parser.h"
#include "../logging/logger.h"

using namespace Papyrus;

// This Include
#include "iniparser.h"

// Implementation

FileParser::CIniparser::CIniparser()
{	
}

FileParser::CIniparser::~CIniparser()
{	
	FileParser::FlushFile(this, false);
}

Bool FileParser::CIniparser::Initialise(const Int8* _path, Bool _create)
{
	m_mapPairs.clear();

	return FileParser::IParser::Initialise(_path, _create);
}

Bool FileParser::CIniparser::ShutDown()
{
	m_mapPairs.clear();
	return true;
}

Bool FileParser::CIniparser::Load(const Int8* _path)
{
	if (0 != _path && 0 == m_filePath) 
	{
		Int16 length = strlen(_path) + 1;
		m_filePath = new Int8[length];
		strcpy_s(m_filePath, length, _path);
	}

	string strLine;
	string strMapKey;
	string strSection;
	string strKey;
	string strValue;

	m_filestream.open(m_filePath);

	if (m_filestream.is_open())
	{
		while(!m_filestream.eof())
		{
			getline(m_filestream, strLine);

			// Reading any comments from the .ini file.
			if (-1 != strLine.find(";"))
			{
				// Do nothing, don't need comments stored anywhere 
				// except in the file itself.
			}
			// if no [ in the line, then it's a key with value, not a section.
			else if (-1 == strLine.find("["))
			{
				if (0 != strLine.length())
				{
					// Finding the position of the equals.
					size_t EqualsLocation = strLine.find("=");
					strKey = strLine.substr(0, EqualsLocation);
					strValue = strLine.substr(EqualsLocation);

					// Trimming any whitespace.
					size_t KeyWhiteSpace = strKey.find(" ");
					strKey = strKey.substr(0, KeyWhiteSpace);

					size_t ValueWhiteSpace = strValue.find(" ");
					strValue = strValue.substr(ValueWhiteSpace + 1);
								
					// Checking if there's any quotes surrounding the value, 
					// and removing them.
					size_t FirstQuotes = strValue.find("\"");
					size_t SecondQuotes = strValue.rfind("\"");

					if (-1 != FirstQuotes) 
					{	
						strValue = strValue.substr(FirstQuotes + 1, SecondQuotes - 1);
					}	

					AddValue(strKey.c_str(), strValue.c_str(), strSection.c_str());
				}				
			}
			else
			{
				// Getting the section name without the [] surrounding it.
				size_t CloseBracket = strLine.find("]");
				strSection = strLine.substr(1, CloseBracket - 1);
			}
		}

		m_filestream.close();
		return true;
	}
	else
	{
		m_filestream.close();
		Logger::Write("FileParser:: Unable to open %s for reading", _path);
		return false;
	}
}

Bool FileParser::CIniparser::Save(const Int8* _path)
{
	if (0 != _path && 0 == m_filePath) 
	{
		Int16 length = strlen(_path) + 1;
		m_filePath = new Int8[length];
		strcpy_s(m_filePath, length, _path);
	}

	map<string, string>::iterator iter;
	iter = m_mapPairs.begin();

	string strNextSection;
	string strSection;
	string strKey = "";
	string strValue = "";
	string strLine;
	size_t PipePosition = 0;

	m_filestream.clear();
	m_filestream.open(m_filePath, ios::out);
	
	if (m_filestream.is_open())
	{
		while (iter != m_mapPairs.end())
		{
			// Finding the location of the | to split the map key Into
			// it's section and key values.
			PipePosition = iter->first.find("|");

			strNextSection = iter->first.substr(0, PipePosition);

			if (strNextSection != strSection)
			{
				strSection = strNextSection;

				m_filestream << endl << "[" + strSection + "]" << endl;
			}

			strKey = iter->first.substr(PipePosition + 1);
			strValue = iter->second;

			if ("" != strKey)
			{
				strLine = strKey + " = " + strValue;
			}
			else
			{
				strLine = "";
			}

			m_filestream << strLine << endl;
			++iter;
		}

		m_filestream.close();
		return true;
	}
	else
	{
		m_filestream.close();
		Logger::Write("FileParser:: Unable to open %s for writing", _path);
		return false;
	}
}

Bool FileParser::CIniparser::AddValue(const Int8* _key, const Bool _value, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	Int8* b = new Int8[MAX_BUFFER];
	sprintf_s(b, MAX_BUFFER, "%b", _value);
	m_mapPairs[CreateMapKey(string(_section), string(_key))] = string(b);
	CLEANARRAY(b);

	return true;
}

Bool FileParser::CIniparser::AddValue(const Int8* _key, const Int8* _value, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	m_mapPairs[CreateMapKey(string(_section), string(_key))] = string(_value);

	return true;
}

Bool FileParser::CIniparser::AddValue(const Int8* _key, Int32 _value, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	Int8* integer = new Int8[MAX_BUFFER];
	sprintf_s(integer, MAX_BUFFER, "%i", _value);
	m_mapPairs[CreateMapKey(string(_section), string(_key))] = string(integer);
	CLEANARRAY(integer);

	return true;
}

Bool FileParser::CIniparser::AddValue(const Int8* _key, UInt32 _value, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	Int8* integer = new Int8[MAX_BUFFER];
	sprintf_s(integer, MAX_BUFFER, "%i", _value);
	m_mapPairs[CreateMapKey(string(_section), string(_key))] = string(integer);
	CLEANARRAY(integer);

	return true;
}

Bool FileParser::CIniparser::AddValue(const Int8* _key, Float32 _value, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	Int8* floatValue = new Int8[MAX_BUFFER];
	sprintf_s(floatValue, MAX_BUFFER, "%f", _value);
	m_mapPairs[CreateMapKey(string(_section), string(_key))] = string(floatValue);
	CLEANARRAY(floatValue);

	return true;
}

Bool FileParser::CIniparser::AddValue(const Int8* _key, const VECTOR3& _value, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	Int8* vector = new Int8[MAX_BUFFER];
	sprintf_s(vector, MAX_BUFFER, "%f, %f, %f", _value.x, _value.y, _value.z);
	m_mapPairs[CreateMapKey(string(_section), string(_key))] = string(vector);
	CLEANARRAY(vector);

	return true;
}

Bool FileParser::CIniparser::AddValue(const Int8* _key, const VECTOR4& _value, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	Int8* vector = new Int8[MAX_BUFFER];
	sprintf_s(vector, MAX_BUFFER, "%f, %f, %f, %f", _value.x, _value.y, _value.z, _value.w);
	m_mapPairs[CreateMapKey(string(_section), string(_key))] = string(vector);
	CLEANARRAY(vector);

	return true;
}

Bool FileParser::CIniparser::DeleteValue(const Int8* _key, const Int8* _section)
{	
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	string strSection = _section;
	string strKey = _key;

	if (m_mapPairs.erase(CreateMapKey(strSection, strKey)) > 0)
	{
		return true;
	}

	return false;
}

Bool FileParser::CIniparser::GetValue(const Int8* _key, Bool& _rbValue, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	string strSection = _section;
	string strKey = _key;

	if (m_mapPairs.end() != m_mapPairs.find(CreateMapKey(strSection, strKey)))
	{
		string strMapValue = m_mapPairs[CreateMapKey(strSection, strKey)];

		// Making sure that the value is lower case.
		for (size_t i = 0; i < strMapValue.length(); ++i)
		{
			strMapValue[i] = tolower(strMapValue[i]);
		}

		if ("true" == strMapValue)
		{
			_rbValue = true;
		}
		else
		{
			_rbValue = false;
		}

		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to find %s:%s", _section, _key);
		return false;
	}
}

Bool FileParser::CIniparser::GetValue(const Int8* _key, Int8** _kcpValue, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	string strSection = _section;
	string strKey = _key;

	if (m_mapPairs.end() != m_mapPairs.find(CreateMapKey(strSection, strKey)))
	{
		const Int8* cpText = m_mapPairs[CreateMapKey(strSection, strKey)].c_str();
		Int32 iLength = strlen(cpText);
		++iLength;
		*_kcpValue = new Int8[iLength];
		sprintf_s(*_kcpValue, iLength, "%s", cpText);
		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to find %s:%s", _section, _key);
		return false;
	}
}

Bool FileParser::CIniparser::GetValue(const Int8* _key, Int32& _riValue, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	string strSection = _section;
	string strKey = _key;

	if (m_mapPairs.end() != m_mapPairs.find(CreateMapKey(strSection, strKey)))
	{
		_riValue = atoi(m_mapPairs[CreateMapKey(strSection, strKey)].c_str());

		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to find %s:%s", _section, _key);
		return false;
	}
}

Bool FileParser::CIniparser::GetValue(const Int8* _key, UInt32& _rufValue, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	string strSection = _section;
	string strKey = _key;

	if (m_mapPairs.end() != m_mapPairs.find(CreateMapKey(strSection, strKey)))
	{
		_rufValue = static_cast<UInt32>(atoi(m_mapPairs[CreateMapKey(strSection, strKey)].c_str()));
		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to find %s:%s", _section, _key);
		return false;
	}
}

Bool FileParser::CIniparser::GetValue(const Int8* _key, Float32& _rfValue, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	string strSection = _section;
	string strKey = _key;

	if (m_mapPairs.end() != m_mapPairs.find(CreateMapKey(strSection, strKey)))
	{
		_rfValue = static_cast<Float32>(atof(m_mapPairs[CreateMapKey(strSection, strKey)].c_str()));
		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to find %s:%s", _section, _key);
		return false;
	}
}

Bool FileParser::CIniparser::GetValue(const Int8* _key, VECTOR3& _rv3Value, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	string strSection = _section;
	string strKey = _key;
	string strResult;
	Int32 iToY;
	Int32 iToZ;
	Int32 iLength;

	if(m_mapPairs.end() != m_mapPairs.find(CreateMapKey(strSection, strKey)))
	{
		strResult = m_mapPairs[CreateMapKey(strSection, strKey)];

		iLength = static_cast<Int32>(strResult.length());

		string strModifier;
		iToY = static_cast<int>(strResult.find(','));

		strModifier = strResult;
		strModifier = strResult.substr(iToY + 1, iLength);

		iToZ = static_cast<int>(strModifier.find(','));
		iToZ += iToY + 1;

		string strX = strResult.substr(0, iToY);
		_rv3Value.x = static_cast<Float32>(atof(strX.c_str()));

		string strY = strResult.substr(iToY + 1, iToZ);
		_rv3Value.y = static_cast<Float32>(atof(strY.c_str()));

		string strZ = strResult.substr(iToZ + 1, iLength);
		_rv3Value.z = static_cast<Float32>(atof(strZ.c_str()));

		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to find %s:%s", _section, _key);
		return false;
	}
}

Bool FileParser::CIniparser::GetValue(const Int8* _key, VECTOR4& _rv4Value, const Int8* _section)
{
	assert(_section != 0 && "_section cannot be 0 for an INI parser.");
	string strSection = _section;
	string strKey = _key;
	string strResult = "";
	Int32 iToY = 0;
	Int32 iToZ = 0;
	Int32 iToW = 0;
	Int32 iLength = 0;

	if(m_mapPairs.end() != m_mapPairs.find(CreateMapKey(strSection, strKey)))
	{
		strResult = m_mapPairs[CreateMapKey(strSection, strKey)];

		iLength = static_cast<Int32>(strResult.length());

		string strModifierZ;
		string strModifierW;
		iToY = static_cast<int>(strResult.find(','));

		strModifierZ = strResult.substr(iToY + 2, iLength);
		iToZ = static_cast<int>(strModifierZ.find(','));
		iToZ += iToY + 2;
		
		strModifierW = strResult.substr(iToZ + 2, iLength);
		iToW = static_cast<int>(strModifierW.find(','));
		iToW += iToZ + 2;

		string strX = strResult.substr(0, iToY - 1);
		_rv4Value.x = static_cast<Float32>(atof(strX.c_str()));

		string strY = strResult.substr(iToY + 2, 4);
		_rv4Value.y = static_cast<Float32>(atof(strY.c_str()));

		string strZ = strResult.substr(iToZ + 2, 4);
		_rv4Value.z = static_cast<Float32>(atof(strZ.c_str()));
		
		string strW = strResult.substr(iToW + 1, 4);
		_rv4Value.w = static_cast<Float32>(atof(strW.c_str()));

		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to find %s:%s", _section, _key);
		return false;
	}
}

string FileParser::CIniparser::CreateMapKey(string& _strSection, string& _strKey)
{
	return(_strSection + "|" + _strKey);
}