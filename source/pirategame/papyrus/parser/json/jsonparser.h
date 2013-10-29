//
// jsonparser.h
//
// This file defines a class for loading JSON files using the rapidjson library.
//
// rapidjson License
//
// Copyright (C) 2011 Milo Yip
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#pragma once

#ifndef __PAPYRUS_FILEPARSER_JSON_H__
#define __PAPYRUS_FILEPARSER_JSON_H__

// Library Includes
#include <fstream>

// Local Includes
#include "..\iparser.h"
#include "rapidjson\document.h"

namespace Papyrus
{
	namespace FileParser
	{
		class CJSONParser : public IParser
		{
			// Member Functions
		public:
			CJSONParser();
			virtual ~CJSONParser();

			virtual Bool Initialise(const Int8* _path, Bool _create = false);
			virtual Bool ShutDown();

			virtual Bool Load(const Int8* _path = 0);
			virtual Bool Save(const Int8* _path = 0);

			virtual Bool AddValue(const Int8* _key, const Bool _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, const Int8* _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, Int32 _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, UInt32 _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, Float32 _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, const VECTOR3& _value, const Int8* _section = 0);
			virtual Bool AddValue(const Int8* _key, const VECTOR4& _value, const Int8* _section = 0);

			virtual Bool DeleteValue(const Int8* _key, const Int8* _section = 0);

			virtual Bool GetValue(const Int8* _key, Bool& _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, Int8** _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, Int32& _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, UInt32& _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, Float32& _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, VECTOR3& _value, const Int8* _section = 0);
			virtual Bool GetValue(const Int8* _key, VECTOR4& _value, const Int8* _section = 0);

			// Member Variables
		private:
			rapidjson::Document			m_json;
			fstream						m_filestream;
		};
	}
}

#endif //__PAPYRUS_FILEPARSER_JSON_H__