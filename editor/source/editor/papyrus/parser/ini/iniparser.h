//
// iniparser.h
//
// This file defines a class for loading .ini files.
//
#pragma once

#ifndef __INIPARSER_H__
#define __INIPARSER_H__

// Library Includes
#include <fstream>
#include <map>
#include <string>

// Local Includes
#include "..\iparser.h"

using namespace std;

// Prototypes
namespace Papyrus
{
	namespace FileParser
	{
		class CIniparser : public IParser
		{
			// Member Functions
		public:
			CIniparser();
			virtual ~CIniparser();

			virtual Bool Initialise(const Int8* _path, Bool _create = false);
			virtual Bool ShutDown();

			virtual Bool Load(const Int8* _path = 0);
			virtual Bool Save(const Int8* _path = 0);

			virtual Bool AddValue(const Int8* _key, const Bool _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, const Int8* _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, Int32 _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, UInt32 _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, Float32 _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, const VECTOR2& _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, const VECTOR3& _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, const VECTOR4& _value, const Int8* _section = 0);

			virtual Bool DeleteValue(const Int8* _key, const Int8* _section = 0);

			virtual Bool GetValue(const Int8* _key, Bool& _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, Int8** _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, Int32& _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, UInt32& _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, Float32& _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, VECTOR2& _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, VECTOR3& _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, VECTOR4& _value, const Int8* _section = 0);

		private:
			/* 
			* This function returns the Section and Key as a single string
			* 	
			* @params	_strSection				The section to be returned
			* @params	_strKey					The key to be returned
			* @return	Returns the section combined before a "|" and then the key after.
			*/
			string CreateMapKey(string& _section, string& _key);

			// Member Variables
		private:
			map<string, string>		m_mapPairs;
			fstream					m_filestream;
		};
	}
}
#endif //__INIPARSER_H__
