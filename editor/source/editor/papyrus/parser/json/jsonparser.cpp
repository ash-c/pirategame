
// Library Includes
#include <sstream>
#include <iostream>

// Local Includes
#include "../parser.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include "rapidjson/stringbuffer.h"

#include "../logging/logger.h"

using namespace Papyrus;

// This Includes
#include "jsonparser.h"

FileParser::CJSONParser::CJSONParser()
{
	
}

FileParser::CJSONParser::~CJSONParser()
{	
	Papyrus::FileParser::FlushFile(this, false);
}


Bool FileParser::CJSONParser::Initialise(const Int8* _path, Bool _create)
{
	m_json.SetObject();

	return Papyrus::FileParser::IParser::Initialise(_path, _create);
}

Bool FileParser::CJSONParser::ShutDown()
{
	return true;
}

Bool FileParser::CJSONParser::Load(const Int8* _path)
{
	if (0 != _path && 0 == m_filePath) 
	{
		Int16 length = strlen(_path) + 1;
		m_filePath = new Int8[length];
		strcpy_s(m_filePath, length, _path);
	}

	string strLine;
	string strJson = "";
	
	m_filestream.clear();
	m_filestream.open(m_filePath);

	if (m_filestream.is_open())
	{
		while (!m_filestream.eof())
		{
			getline(m_filestream, strLine);
			strJson += strLine;
		}

		m_json.Parse<0>(strJson.c_str());
		assert(m_json.IsObject());
		m_filestream.close();
		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to open %s for reading", _path);
		m_filestream.close();
		return false;
	}
}

Bool FileParser::CJSONParser::Save(const Int8* _path)
{
	if (0 != _path && 0 == m_filePath) 
	{
		Int16 length = strlen(_path) + 1;
		m_filePath = new Int8[length];
		strcpy_s(m_filePath, length, _path);
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	m_json.Accept(writer);

	m_filestream.clear();
	m_filestream.open(m_filePath, ios::out);
	
	if (m_filestream.is_open())
	{
		string contents = buffer.GetString();
		m_filestream << contents;
		m_filestream.close();
		return true;
	}
	else 
	{
		Logger::Write("FileParser:: Unable to open %s for writing", _path);
		m_filestream.close();
		return false;
	}
}

Bool FileParser::CJSONParser::AddValue(const Int8* _key, const Bool _value, const Int8* _section)
{
	if (!m_json.HasMember(_key))
	{
		m_json.AddMember(_key, m_json.GetAllocator(), rapidjson::Value(_value), m_json.GetAllocator());
	}
	else
	{
		m_json[_key].SetBool(_value);
	}
	return true;
}

Bool FileParser::CJSONParser::AddValue(const Int8* _key, const Int8* _value, const Int8* _section)
{
	if (!m_json.HasMember(_key))
	{
		m_json.AddMember(_key, m_json.GetAllocator(), rapidjson::Value(_value, m_json.GetAllocator()), m_json.GetAllocator());
	}
	else
	{
		m_json[_key].SetString(_value, strlen(_value), m_json.GetAllocator());
	}
	return true;
}

Bool FileParser::CJSONParser::AddValue(const Int8* _key, Int32 _value, const Int8* _section)
{
	if (!m_json.HasMember(_key))
	{
		m_json.AddMember(_key, m_json.GetAllocator(), rapidjson::Value(_value), m_json.GetAllocator());
	}
	else
	{
		m_json[_key].SetInt(_value);
	}
	return true;
}

Bool FileParser::CJSONParser::AddValue(const Int8* _key, UInt32 _value, const Int8* _section)
{
	if (!m_json.HasMember(_key))
	{
		m_json.AddMember(_key, m_json.GetAllocator(), rapidjson::Value(_value), m_json.GetAllocator());
	}
	else
	{
		m_json[_key].SetUint(_value);
	}
	return true;
}

Bool FileParser::CJSONParser::AddValue(const Int8* _key, Float32 _value, const Int8* _section)
{
	// Casting to Float64 as rapidjson deals with doubles, not floats.
	if (!m_json.HasMember(_key))
	{
		m_json.AddMember(_key, m_json.GetAllocator(), rapidjson::Value(static_cast<Float64>(_value)), m_json.GetAllocator());
	}
	else
	{
		m_json[_key].SetDouble(_value);
	}
	return true;
}

Bool FileParser::CJSONParser::AddValue(const Int8* _key, const VECTOR2& _value, const Int8* _section)
{
	rapidjson::Document::AllocatorType& allocator = m_json.GetAllocator();
	rapidjson::Value arr(rapidjson::Type::kArrayType);
	arr.PushBack(_value.x, allocator).PushBack(_value.y, allocator);
	m_json.AddMember(_key, allocator, arr, allocator);

	return true;
}

Bool FileParser::CJSONParser::AddValue(const Int8* _key, const VECTOR3& _value, const Int8* _section)
{
	rapidjson::Document::AllocatorType& allocator = m_json.GetAllocator();
	rapidjson::Value arr(rapidjson::Type::kArrayType);
	arr.PushBack(_value.x, allocator).PushBack(_value.y, allocator).PushBack(_value.z, allocator);
	m_json.AddMember(_key, allocator, arr, allocator);

	return true;
}

Bool FileParser::CJSONParser::AddValue(const Int8* _key, const VECTOR4& _value, const Int8* _section)
{
	rapidjson::Document::AllocatorType& allocator = m_json.GetAllocator();
	rapidjson::Value arr(rapidjson::Type::kArrayType);
	arr.PushBack(_value.x, allocator).PushBack(_value.y, allocator).PushBack(_value.z, allocator).PushBack(_value.w, allocator);
	m_json.AddMember(_key, allocator, arr, allocator);

	return true;
}

Bool FileParser::CJSONParser::DeleteValue(const Int8* _key, const Int8* _section)
{
	return m_json.RemoveMember(_key);
}

Bool FileParser::CJSONParser::GetValue(const Int8* _key, Bool& _value, const Int8* _section)
{
	if (m_json[_key].IsBool())
	{
		_value = m_json[_key].GetBool();
		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to get %s, wrong type or doesn't exist", _key);
		return false;
	}
}

Bool FileParser::CJSONParser::GetValue(const Int8* _key, Int8** _value, const Int8* _section)
{
	if (m_json[_key].IsString())
	{
		const Int8* cp = m_json[_key].GetString();
		Int32 length = m_json[_key].GetStringLength() + 1;
		*_value = new Int8[length];
		sprintf_s(*_value, length, "%s", cp);
		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to get %s, wrong type or doesn't exist", _key);
		return false;
	}
}

Bool FileParser::CJSONParser::GetValue(const Int8* _key, Int32& _value, const Int8* _section)
{
	if (m_json[_key].IsInt())
	{
		_value = m_json[_key].GetInt();
		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to get %s, wrong type or doesn't exist", _key);
		return false;
	}
}

Bool FileParser::CJSONParser::GetValue(const Int8* _key, UInt32& _value, const Int8* _section)
{
	if (m_json[_key].IsUint())
	{
		_value = m_json[_key].GetUint();
		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to get %s, wrong type or doesn't exist", _key);
		return false;
	}
}

Bool FileParser::CJSONParser::GetValue(const Int8* _key, Float32& _value, const Int8* _section)
{
	if (m_json[_key].IsDouble())
	{
		_value = static_cast<Float32>(m_json[_key].GetDouble());
		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to get %s, wrong type or doesn't exist", _key);
		return false;
	}
}

Bool FileParser::CJSONParser::GetValue(const Int8* _key, VECTOR2& _value, const Int8* _section)
{
	if (m_json[_key].IsArray())
	{
		const rapidjson::Value& arrValues = m_json[_key];

		rapidjson::SizeType i = 0;
		_value.x = static_cast<Float32>(arrValues[i++].GetDouble());
		_value.y = static_cast<Float32>(arrValues[i++].GetDouble());

		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to get %s, wrong type or doesn't exist", _key);
		return false;
	}
}

Bool FileParser::CJSONParser::GetValue(const Int8* _key, VECTOR3& _value, const Int8* _section)
{
	if (m_json[_key].IsArray())
	{
		const rapidjson::Value& arrValues = m_json[_key];

		rapidjson::SizeType i = 0;
		_value.x = static_cast<Float32>(arrValues[i++].GetDouble());
		_value.y = static_cast<Float32>(arrValues[i++].GetDouble());
		_value.z = static_cast<Float32>(arrValues[i++].GetDouble());

		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to get %s, wrong type or doesn't exist", _key);
		return false;
	}
}

Bool FileParser::CJSONParser::GetValue(const Int8* _key, VECTOR4& _value, const Int8* _section)
{
	if (m_json[_key].IsArray())
	{
		const rapidjson::Value& arrValues = m_json[_key];

		rapidjson::SizeType i = 0;
		_value.x = static_cast<Float32>(arrValues[i++].GetDouble());
		_value.y = static_cast<Float32>(arrValues[i++].GetDouble());
		_value.z = static_cast<Float32>(arrValues[i++].GetDouble());
		_value.w = static_cast<Float32>(arrValues[i++].GetDouble());

		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to get %s, wrong type or doesn't exist", _key);
		return false;
	}
}