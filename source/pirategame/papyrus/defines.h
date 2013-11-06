
#pragma once

#ifndef __DEFINES_H__
#define __DEFINES_H__

// Library Includes
#include <ctime>
#include <cassert>

// Local Includes
using namespace std;

////////////////////
// Constants
////////////////////
#define HIBIT			0x8000
#define INVALID_ID		-1
#define MAX_BUFFER		256

////////////////////
// Types
////////////////////

typedef bool					Bool;
typedef char					Int8;
typedef unsigned char			UInt8;
typedef wchar_t					WInt8;
typedef short					Int16;
typedef unsigned short			UInt16;
typedef int						Int32;
typedef unsigned int			UInt32;
typedef float					Float32;
typedef double					Float64;

////////////////////
// Structs
////////////////////
typedef struct _TVECTOR3 
{
	Float32 x;
	Float32 y;
	Float32 z;
} VECTOR3;

typedef struct _TVECTOR4
{
	Float32 x;
	Float32 y;
	Float32 z;
	Float32 w;
} VECTOR4;

////////////////////
// Macros
////////////////////

#define _XSTR( msg ) #msg
#define _TOSTR( msg ) _XSTR( msg )
#define todo(msg) message("//// TODO: " msg)

#define VALIDATE(a) if (!a) return false;

#define CREATEPOINTER(Pointer,Class) if (Pointer == 0) { Pointer = new Class; assert(Pointer); }
#define CLEANRELEASE(Object) if (Object != 0) { Object->Release(); }
#define CLEANDELETE(Object) if (Object != 0) { delete Object; Object = 0; }
#define CLEANARRAY(Object) if (Object != 0) { delete[] Object; Object = 0; }

#define PY_RELEASE(Object) if (0 != Object) { Object->Release(); Object->ShutDown(); delete Object; Object = 0; }

#define CLEANVECTOR(VectorObject) for (UInt32 i = 0; i < VectorObject.size(); ++i)\
	{\
	CLEANDELETE(VectorObject[i]);\
	}\
	VectorObject.clear();

#define CLEANMAP(MapObject) for (UInt32 i = 0; i < MapObject.size(); ++i)\
	{\
		CLEANDELETE(MapObject[i]);\
	}\
	MapObject.clear();

#define CLEANLIST(ListObject) Int32 iLength = static_cast<Int32>(ListObject.size());\
for(Int32 i = 0; i < iLength; ++i)\
{\
	CLEANDELETE(ListObject.back())\
	ListObject.pop_back();\
}\
ListObject.clear();

#define VWPRINTF(Buffer,Format,Args) Int32 iLength = 0;\
iLength = _vscwprintf((Format), (Args)) + 1;\
assert(iLength);\
(Buffer) = new WInt8[iLength];\
assert((Buffer));\
vswprintf_s((Buffer), iLength, (Format), (Args));\
assert((Buffer));

#endif // __DEFINES_H__