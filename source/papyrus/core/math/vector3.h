#pragma once

#ifndef __PAPYRUS_CORE_MATH_VECTOR3_H__
#define __PAPYRUS_CORE_MATH_VECTOR3_H__

// Local Includes

struct _TVECTOR3
{
	_TVECTOR3()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
	{
	}

	_TVECTOR3(float x, float y, float z)
		: x(x)
		, y(y)
		, z(z)
	{
	}

	_TVECTOR3& operator+= (const _TVECTOR3& _rhs)
	{
		x += _rhs.x;
		y += _rhs.y;
		z += _rhs.z;
		return *this;
	}

	_TVECTOR3& operator-= (const _TVECTOR3& _rhs)
	{
		x -= _rhs.x;
		y -= _rhs.y;
		z -= _rhs.z;
		return *this;
	}

	_TVECTOR3& operator*= (const _TVECTOR3& _rhs)
	{
		x *= _rhs.x;
		y *= _rhs.y;
		z *= _rhs.z;
		return *this;
	}

	_TVECTOR3& operator/= (const _TVECTOR3& _rhs)
	{
		x /= _rhs.x;
		y /= _rhs.y;
		z /= _rhs.z;
		return *this;
	}

	bool operator== (const _TVECTOR3& _rhs)
	{
		return (_rhs.x == x && _rhs.y == y && _rhs.z == z ? true : false);
	}

	const _TVECTOR3 operator+ (const _TVECTOR3& _rhs)
	{
		_TVECTOR3 result = *this;
		result += _rhs;
		return result;
	}

	const _TVECTOR3 operator- (const _TVECTOR3& _rhs)
	{
		_TVECTOR3 result = *this;
		result -= _rhs;
		return result;
	}

	const _TVECTOR3 operator* (const float& _f)
	{
		_TVECTOR3 result = *this;
		result.x *= _f;
		result.y *= _f;
		result.z *= _f;
		return result;
	}

	float		x;
	float		y;
	float		z;
};

#endif // __PAPYRUS_CORE_MATH_VECTOR3_H__