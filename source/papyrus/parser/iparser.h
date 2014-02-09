//
// iparser.h
//
// This file defines an interface for IParser objects.
//
#pragma once

#ifndef __PAPYRUS_FILEPARSER_PARSER_H__
#define __PAPYRUS_FILEPARSER_PARSER_H__

// Library Includes
#include <cstring>

// Local Includes
#include "../defines.h"
#include "../core/utility/refcount.h"

namespace Papyrus
{
	namespace FileParser
	{
		class IParser : public TRefCount<IParser>
		{
			// Member Functions
		public:
			// Default constructor.
			IParser() 
			{	
				
			}
			// Default destructor.
			virtual ~IParser() 
			{
			}

			/*
			* Initialises the parser.
			*
			* @param	_path		Filepath of the file being loaded/created.
			* @param	_create		Whether or not this parser is creating a blank parser.
			* @return	Returns true on successfully loading a parser or creating a new one. False otherwise.
			*/
			virtual Bool Initialise(const Int8* _path, Bool _create = false)
			{
				Int16 length = strlen(_path) + 1;
				m_filePath = new Int8[length];
				strcpy_s(m_filePath, length, _path);
				return true;
			}

			/*
			* Shuts down the parser and cleans memory.
			*
			* @return	Returns true when successfully shutdown.
			*/
			virtual Bool ShutDown() = 0;

			/*
			* Loads a file into memory and parses the values stored in it.
			*
			* @param	_path		Path to the file to load.
			* @return	Returns true if successfully loaded. False otherwise.
			*/
			virtual Bool Load(const Int8* _path = 0) = 0;

			/*
			* Saves the parser out to file.
			*
			* @param	_path		Path to save to. If 0, uses the original path that the parser was loaded with.
			* @return	Returns true if successfully saved to file. False otherwise.
			*/
			virtual Bool Save(const Int8* _path = 0) = 0;

			/*
			* Adds a boolean value to the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The value to be stored.
			* @return	Returns true if successfully updated.
			*/
			virtual Bool AddValue(const Int8* _key, const Bool _value, const Int8* _section = 0) = 0;

			/*
			* Adds a character string to the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The value to be stored.
			* @return	Returns true if successfully updated.
			*/
			virtual Bool AddValue(const Int8* _key, const Int8* _value, const Int8* _section = 0) = 0;

			/*
			* Adds an integer value to the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The value to be stored.
			* @return	Returns true if successfully updated.
			*/
			virtual Bool AddValue(const Int8* _key, Int32 _value, const Int8* _section = 0) = 0;
			
			/*
			* Adds an unsigned integer value to the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The value to be stored.
			* @return	Returns true if successfully updated.
			*/
			virtual Bool AddValue(const Int8* _key, UInt32 _value, const Int8* _section = 0) = 0;

			/*
			* Adds a float value to the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The value to be stored.
			* @return	Returns true if successfully updated.
			*/
			virtual Bool AddValue(const Int8* _key, Float32 _value, const Int8* _section = 0) = 0;

			/*
			* Adds a vector2 to the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The value to be stored.
			* @return	Returns true if successfully updated.
			*/
			virtual Bool AddValue(const Int8* _key, const VECTOR2& _value, const Int8* _section = 0) = 0;

			/*
			* Adds a vector3 to the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The value to be stored.
			* @return	Returns true if successfully updated.
			*/
			virtual Bool AddValue(const Int8* _key, const VECTOR3& _value, const Int8* _section = 0) = 0;

			/*
			* Adds a vector4 to the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The value to be stored.
			* @return	Returns true if successfully updated.
			*/
			virtual Bool AddValue(const Int8* _key, const VECTOR4& _value, const Int8* _section = 0) = 0;

			/*
			* Deletes a value from the parser.
			*
			* @param	_section		Section of the parser the key is stored under.
			* @param	_key			Key attribute to be deleted.
			* @return	Returns true if successfully deleted.
			*/
			virtual Bool DeleteValue(const Int8* _key, const Int8* _section = 0) = 0;

			/*
			* Gets a boolean value from the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The boolean value to be returned.
			* @return	The _value parameter will be filled out with the value being returned.
			*/
			virtual Bool GetValue(const Int8* _key, Bool& _value, const Int8* _section = 0) = 0;

			/*
			* Gets a character string from the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The character string to be returned.
			* @return	The _value parameter will be filled out with the value being returned.
			*/
			virtual Bool GetValue(const Int8* _key, Int8** _value, const Int8* _section = 0) = 0;

			/*
			* Gets a signed integer value from the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The signed integer to be returned.
			* @return	The _value parameter will be filled out with the value being returned.
			*/
			virtual Bool GetValue(const Int8* _key, Int32& _value, const Int8* _section = 0) = 0;

			/*
			* Gets a unsigned integer value from the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The unsigned integer to be returned.
			* @return	The _value parameter will be filled out with the value being returned.
			*/
			virtual Bool GetValue(const Int8* _key, UInt32& _value, const Int8* _section = 0) = 0;

			/*
			* Gets a float value from the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The float value to be returned.
			* @return	The _value parameter will be filled out with the value being returned.
			*/
			virtual Bool GetValue(const Int8* _key, Float32& _value, const Int8* _section = 0) = 0;

			/*
			* Gets a vector2 value from the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The vector2 value to be returned.
			* @return	The _value parameter will be filled out with the value being returned.
			*/
			virtual Bool GetValue(const Int8* _key, VECTOR2& _value, const Int8* _section = 0) = 0;

			/*
			* Gets a vector3 value from the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The vector3 value to be returned.
			* @return	The _value parameter will be filled out with the value being returned.
			*/
			virtual Bool GetValue(const Int8* _key, VECTOR3& _value, const Int8* _section = 0) = 0;

			/*
			* Gets a vector4 value from the parser.
			*
			* @param	_section		Section of the parser to store the key/value under.
			* @param	_key			Key attribute.
			* @param	_value			The vector4 value to be returned.
			* @return	The _value parameter will be filled out with the value being returned.
			*/
			virtual Bool GetValue(const Int8* _key, VECTOR4& _value, const Int8* _section = 0) = 0;

			// Member Variables
		protected:
		};
	}
}

#endif // __PAPYRUS_FILEPARSER_PARSER_H__