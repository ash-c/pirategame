
#pragma once

#ifndef __DEFINES_H__
#define __DEFINES_H__

// Library Includes
#include <cassert>

// Local Includes
#include "core\math\vectormath.h"

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
typedef Papyrus::Math::_TVECTOR2	VECTOR2;
typedef Papyrus::Math::_TVECTOR3	VECTOR3;
typedef Papyrus::Math::_TVECTOR4	VECTOR4;

////////////////////
// Macros
////////////////////

#define _XSTR( msg ) #msg
#define _TOSTR( msg ) _XSTR( msg )
#define todo(msg) message("//// TODO: " msg)

#define VALIDATE(a) if (!a) return false;

#define CREATEPOINTER(Pointer,Class) if (Pointer == 0) { Pointer = new Class; assert(Pointer); }
#define CLEANDELETE(Object) if (Object != 0) { delete Object; Object = 0; }
#define CLEANARRAY(Object) if (Object != 0) { delete[] Object; Object = 0; }

#define PY_DELETE_RELEASE(Object) if (0 != Object) { Object->Release(); Object->ShutDown(); delete Object; Object = 0; }
#define PY_SAFE_RELEASE(Object) if (0 != Object) { Object->Release(); Object = 0; }

#define PY_CLEANARRAY(Array, Length) \
for (Int16 i = 0; i < Length; ++i)\
{\
	if (0 != Array[i])\
	{\
		Array[i]->Release();\
		Array[i]->ShutDown();\
		CLEANDELETE(Array[i]);\
	}\
}\

#endif // __DEFINES_H__