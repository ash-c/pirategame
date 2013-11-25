#pragma once

#ifndef __PAPYRUS_CORE_MATH_VECTOR4_H__
#define __PAPYRUS_CORE_MATH_VECTOR4_H__

// Local Includes

namespace Papyrus
{
	namespace Math
	{
		struct _TVECTOR4
		{
			_TVECTOR4()
				: x(0.0f)
				, y(0.0f)
				, z(0.0f)
				, w(0.0f)
			{
			}

			_TVECTOR4(float x, float y, float z, float w)
				: x(x)
				, y(y)
				, z(z)
				, w(w)
			{
			}

			_TVECTOR4& operator+= (const _TVECTOR4& _rhs)
			{
				x += _rhs.x;
				y += _rhs.y;
				z += _rhs.z;
				w += _rhs.w;
				return *this;
			}

			_TVECTOR4& operator-= (const _TVECTOR4& _rhs)
			{
				x -= _rhs.x;
				y -= _rhs.y;
				z -= _rhs.z;
				w -= _rhs.w;
				return *this;
			}

			_TVECTOR4& operator*= (const _TVECTOR4& _rhs)
			{
				x *= _rhs.x;
				y *= _rhs.y;
				z *= _rhs.z;
				w *= _rhs.w;
				return *this;
			}

			_TVECTOR4& operator/= (const _TVECTOR4& _rhs)
			{
				x /= _rhs.x;
				y /= _rhs.y;
				z /= _rhs.z;
				w /= _rhs.w;
				return *this;
			}

			const _TVECTOR4 operator+ (const _TVECTOR4& _rhs)
			{
				_TVECTOR4 result = *this;
				result += _rhs;
				return result;
			}

			const _TVECTOR4 operator- (const _TVECTOR4& _rhs)
			{
				_TVECTOR4 result = *this;
				result -= _rhs;
				return result;
			}

			const _TVECTOR4 operator* (const float& _f)
			{
				_TVECTOR4 result = *this;
				result.x *= _f;
				result.y *= _f;
				result.z *= _f;
				result.w *= _f;
				return result;
			}

			float		x;
			float		y;
			float		z;
			float		w;
		};
	}
}

#endif // __PAPYRUS_CORE_MATH_VECTOR4_H__