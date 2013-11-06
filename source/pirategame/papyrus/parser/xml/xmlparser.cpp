
// Library Includes
#include <sstream>
#include <iostream>

// Local Includes
#include "../parser.h"
#include "rapidxml/rapidxml_print.hpp"

#include "../logging/logger.h"

using namespace Papyrus;

// This Includes
#include "xmlparser.h"

FileParser::CXMLParser::CXMLParser()
{
	
}

FileParser::CXMLParser::~CXMLParser()
{	
	Papyrus::FileParser::FlushFile(this, false);
}


Bool FileParser::CXMLParser::Initialise(const Int8* _path, Bool _create)
{
	m_xml.clear();
	return FileParser::IParser::Initialise(_path, _create);
}

Bool FileParser::CXMLParser::ShutDown()
{
	m_xml.clear();
	return true;
}

Bool FileParser::CXMLParser::Load(const Int8* _path)
{
	if (0 != _path && 0 == m_filePath) 
	{
		Int16 length = strlen(_path) + 1;
		m_filePath = new Int8[length];
		strcpy_s(m_filePath, length, _path);
	}

	string strLine;
	m_strXml = "";
	
	m_filestream.clear();
	m_filestream.open(m_filePath);

	if (m_filestream.is_open())
	{
		while (!m_filestream.eof())
		{
			getline(m_filestream, strLine);
			m_strXml += strLine;
		}

		m_xml.parse<0>(&m_strXml[0]);
		
		m_filestream.close();
		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to load %s for reading", _path);
		m_filestream.close();
		return false;
	}
}

Bool FileParser::CXMLParser::Save(const Int8* _path)
{
	if (0 != _path && 0 == m_filePath) 
	{
		Int16 length = strlen(_path) + 1;
		m_filePath = new Int8[length];
		strcpy_s(m_filePath, length, _path);
	}

	m_filestream.clear();
	m_filestream.open(m_filePath, ios::out);
	
	if (m_filestream.is_open())
	{
		cout << m_xml << endl;
		m_filestream << m_xml;
		m_filestream.close();
		return true;
	}
	else
	{
		Logger::Write("FileParser:: Unable to load %s for writing", _path);
		m_filestream.close();
		return false;
	}
}

Bool FileParser::CXMLParser::AddValue(const Int8* _key, const Bool _value, const Int8* _section)
{
	Int8 buffer[MAX_BUFFER];
	sprintf_s(buffer, MAX_BUFFER, "%i", _value);
	rapidxml::xml_node<> *node = m_xml.allocate_node(rapidxml::node_type::node_element, m_xml.allocate_string(_key), m_xml.allocate_string(buffer));
	m_xml.append_node(node);
	return true;
}

Bool FileParser::CXMLParser::AddValue(const Int8* _key, const Int8* _value, const Int8* _section)
{
	rapidxml::xml_node<> *node = m_xml.allocate_node(rapidxml::node_type::node_element, m_xml.allocate_string(_key), m_xml.allocate_string(_value));
	m_xml.append_node(node);
	return true;
}

Bool FileParser::CXMLParser::AddValue(const Int8* _key, Int32 _value, const Int8* _section)
{
	Int8 buffer[MAX_BUFFER];
	sprintf_s(buffer, MAX_BUFFER, "%i", _value);
	rapidxml::xml_node<> *node = m_xml.allocate_node(rapidxml::node_type::node_element, m_xml.allocate_string(_key), m_xml.allocate_string(buffer));
	m_xml.append_node(node);
	return true;
}

Bool FileParser::CXMLParser::AddValue(const Int8* _key, UInt32 _value, const Int8* _section)
{
	Int8 buffer[MAX_BUFFER];
	sprintf_s(buffer, MAX_BUFFER, "%u", _value);
	rapidxml::xml_node<> *node = m_xml.allocate_node(rapidxml::node_type::node_element, m_xml.allocate_string(_key), m_xml.allocate_string(buffer));
	m_xml.append_node(node);
	return true;
}

Bool FileParser::CXMLParser::AddValue(const Int8* _key, Float32 _value, const Int8* _section)
{
	Int8 buffer[MAX_BUFFER];
	sprintf_s(buffer, MAX_BUFFER, "%f", _value);
	rapidxml::xml_node<> *node = m_xml.allocate_node(rapidxml::node_type::node_element, m_xml.allocate_string(_key), m_xml.allocate_string(buffer));
	m_xml.append_node(node);
	return true;
}

Bool FileParser::CXMLParser::AddValue(const Int8* _key, const VECTOR3& _value, const Int8* _section)
{
	rapidxml::xml_node<> *node = m_xml.allocate_node(rapidxml::node_type::node_element, m_xml.allocate_string(_key), m_xml.allocate_string(_key));
	m_xml.append_node(node);
	Int8 buffer[MAX_BUFFER];
	sprintf_s(buffer, MAX_BUFFER, "%f", _value.x);
	node->append_attribute(m_xml.allocate_attribute(m_xml.allocate_string("x"), m_xml.allocate_string(buffer)));
	sprintf_s(buffer, MAX_BUFFER, "%f", _value.y);
	node->append_attribute(m_xml.allocate_attribute(m_xml.allocate_string("y"), m_xml.allocate_string(buffer)));
	sprintf_s(buffer, MAX_BUFFER, "%f", _value.z);
	node->append_attribute(m_xml.allocate_attribute(m_xml.allocate_string("z"), m_xml.allocate_string(buffer)));	
	return true;
}

Bool FileParser::CXMLParser::AddValue(const Int8* _key, const VECTOR4& _value, const Int8* _section)
{
	rapidxml::xml_node<> *node = m_xml.allocate_node(rapidxml::node_type::node_element, m_xml.allocate_string(_key), m_xml.allocate_string(_key));
	m_xml.append_node(node);
	Int8 buffer[MAX_BUFFER];
	sprintf_s(buffer, MAX_BUFFER, "%f", _value.x);
	node->append_attribute(m_xml.allocate_attribute(m_xml.allocate_string("x"), m_xml.allocate_string(buffer)));
	sprintf_s(buffer, MAX_BUFFER, "%f", _value.y);
	node->append_attribute(m_xml.allocate_attribute(m_xml.allocate_string("y"), m_xml.allocate_string(buffer)));
	sprintf_s(buffer, MAX_BUFFER, "%f", _value.z);
	node->append_attribute(m_xml.allocate_attribute(m_xml.allocate_string("z"), m_xml.allocate_string(buffer)));	
	sprintf_s(buffer, MAX_BUFFER, "%f", _value.w);
	node->append_attribute(m_xml.allocate_attribute(m_xml.allocate_string("w"), m_xml.allocate_string(buffer)));
	return true;
}

Bool FileParser::CXMLParser::DeleteValue(const Int8* _key, const Int8* _section)
{
	m_xml.remove_node(m_xml.first_node(_key));
	return true;
}

Bool FileParser::CXMLParser::GetValue(const Int8* _key, Bool& _value, const Int8* _section)
{
	rapidxml::xml_node<>* node = m_xml.first_node(_key);
	if (nullptr == node)
	{
		Logger::Write("FileParser:: Unable to get %s, doesn't exist", _key);
		return false;
	}
	else
	{
		_value = !strcmp(node->value(), "1");
		return true;
	}
}

Bool FileParser::CXMLParser::GetValue(const Int8* _key, Int8** _value, const Int8* _section)
{
	rapidxml::xml_node<>* node = m_xml.first_node(_key);
	if (nullptr == node)
	{
		Logger::Write("FileParser:: Unable to get %s, doesn't exist", _key);
		return false;
	}
	else
	{
		Int16 length = strlen(node->value()) + 1;
		*_value = new Int8[length];
		sprintf_s(*_value, length, "%s", node->value());
		return true;
	}
}

Bool FileParser::CXMLParser::GetValue(const Int8* _key, Int32& _value, const Int8* _section)
{
	rapidxml::xml_node<>* node = m_xml.first_node(_key);
	if (nullptr == node)
	{
		Logger::Write("FileParser:: Unable to get %s, doesn't exist", _key);
		return false;
	}
	else
	{
		_value = atoi(node->value());
		return true;
	}
}

Bool FileParser::CXMLParser::GetValue(const Int8* _key, UInt32& _value, const Int8* _section)
{
	rapidxml::xml_node<>* node = m_xml.first_node(_key);
	if (nullptr == node)
	{
		Logger::Write("FileParser:: Unable to get %s, doesn't exist", _key);
		return false;
	}
	else
	{
		_value = atoi(node->value());
		return true;
	}
}

Bool FileParser::CXMLParser::GetValue(const Int8* _key, Float32& _value, const Int8* _section)
{
	rapidxml::xml_node<>* node = m_xml.first_node(_key);
	if (nullptr == node)
	{
		Logger::Write("FileParser:: Unable to get %s, doesn't exist", _key);
		return false;
	}
	else
	{
		_value = static_cast<Float32>(atof(node->value()));
		return true;
	}
}

Bool FileParser::CXMLParser::GetValue(const Int8* _key, VECTOR3& _value, const Int8* _section)
{
	rapidxml::xml_node<>* node = m_xml.first_node(_key);
	if (nullptr == node)
	{
		Logger::Write("FileParser:: Unable to get %s, doesn't exist", _key);
		return false;
	}
	else
	{
		_value.x = static_cast<Float32>(atof(node->first_attribute("x")->value()));
		_value.y = static_cast<Float32>(atof(node->first_attribute("y")->value()));
		_value.z = static_cast<Float32>(atof(node->first_attribute("z")->value()));
		return true;
	}
}

Bool FileParser::CXMLParser::GetValue(const Int8* _key, VECTOR4& _value, const Int8* _section)
{
	rapidxml::xml_node<>* node = m_xml.first_node(_key);
	if (nullptr == node)
	{
		Logger::Write("FileParser:: Unable to get %s, doesn't exist", _key);
		return false;
	}
	else
	{
		_value.x = static_cast<Float32>(atof(node->first_attribute("x")->value()));
		_value.y = static_cast<Float32>(atof(node->first_attribute("y")->value()));
		_value.z = static_cast<Float32>(atof(node->first_attribute("z")->value()));
		_value.w = static_cast<Float32>(atof(node->first_attribute("w")->value()));
		return true;
	}
}