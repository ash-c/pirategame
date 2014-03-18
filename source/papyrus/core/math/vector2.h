#pragma once

#ifndef __PAPYRUS_CORE_MATH_VECTOR2_H__
#define __PAPYRUS_CORE_MATH_VECTOR2_H__

struct _TVECTOR2
{
	_TVECTOR2()
		: x(0.0f)
		, y(0.0f)
	{
	}

	_TVECTOR2(float x, float y)
		: x(x)
		, y(y)
	{
	}

	_TVECTOR2& operator+= (const _TVECTOR2& _rhs)
	{
		x += _rhs.x;
		y += _rhs.y;
		return *this;
	}

	_TVECTOR2& operator-= (const _TVECTOR2& _rhs)
	{
		x -= _rhs.x;
		y -= _rhs.y;
		return *this;
	}

	_TVECTOR2& operator*= (const _TVECTOR2& _rhs)
	{
		x *= _rhs.x;
		y *= _rhs.y;
		return *this;
	}

	_TVECTOR2& operator/= (const _TVECTOR2& _rhs)
	{
		x /= _rhs.x;
		y /= _rhs.y;
		return *this;
	}

	bool operator== (const _TVECTOR2& _rhs)
	{
		return (_rhs.x == x && _rhs.y == y ? true : false);
	}

	bool operator> (const _TVECTOR2& _rhs)
	{
		return (x > _rhs.x && y > _rhs.y ? true : false);
	}

	bool operator< (const _TVECTOR2& _rhs)
	{
		return (x < _rhs.x && y < _rhs.y ? true : false);
	}

	bool operator>= (const _TVECTOR2& _rhs)
	{
		return (x >= _rhs.x && y >= _rhs.y ? true : false);
	}

	bool operator<= (const _TVECTOR2& _rhs)
	{
		return (x <= _rhs.x && y <= _rhs.y ? true : false);
	}

	_TVECTOR2& operator= (const _TVECTOR2& _rhs)
	{
		x = _rhs.x;
		y = _rhs.y;
		return *this;
	}

	const _TVECTOR2 operator+ (const _TVECTOR2& _rhs) const
	{
		_TVECTOR2 result = *this;
		result += _rhs;
		return result;
	}

	const _TVECTOR2 operator- (const _TVECTOR2& _rhs) const
	{
		_TVECTOR2 result = *this;
		result -= _rhs;
		return result;
	}

	_TVECTOR2& operator*= (const float& _f)
	{
		x *= _f;
		y *= _f;
		return *this;
	}

	_TVECTOR2& operator+= (const float& _f)
	{
		x += _f;
		y += _f;
		return *this;
	}

	_TVECTOR2& operator-= (const float& _f)
	{
		x -= _f;
		y -= _f;
		return *this;
	}

	const _TVECTOR2 operator* (const float& _f) const
	{
		_TVECTOR2 result = *this;
		result.x *= _f;
		result.y *= _f;
		return result;
	}

	const _TVECTOR2 operator/ (const float& _f) const
	{
		_TVECTOR2 result = *this;
		result.x /= _f;
		result.y /= _f;
		return result;
	}

	float		x;
	float		y;
};

#endif // __PAPYRUS_CORE_MATH_VECTOR2_H__