#pragma once

#ifndef __PAPYRUS_MATH_VECTORMATH_H__
#define __PAPYRUS_MATH_VECTORMATH_H__

// Library Includes
#include <cmath>

// Local Includes
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

namespace Papyrus
{
	namespace Math
	{
		/*
		*	VECTOR2 Functions
		*/
		static float Magnitude(const _TVECTOR2& _krA)
		{
			return(sqrt(pow(_krA.x,2) + pow(_krA.y,2)));
		}

		static float DotProduct(const _TVECTOR2& _krA, const _TVECTOR2& _krB)
		{
			return(_krA.x * _krB.x + _krA.y * _krB.y);
		}

		static _TVECTOR2& Normalise(const _TVECTOR2& _krA, _TVECTOR2& _rResultant)
		{
			float fMagnitudeOfA = Magnitude(_krA);

			_rResultant.x = _krA.x / fMagnitudeOfA;
			_rResultant.y = _krA.y / fMagnitudeOfA;
			return(_rResultant);
		}

		// This function projects _krA onto _krB.
		static _TVECTOR2& Projection(const _TVECTOR2& _krA, const _TVECTOR2& _krB, 
								   _TVECTOR2& _rResultant)
		{
			// Function projects _krA onto _krB.
			float fProjectionScalar = DotProduct(_krA, _krB) / DotProduct(_krB, _krB);

			_rResultant.x = fProjectionScalar * _krB.x;
			_rResultant.y = fProjectionScalar * _krB.y;

			return(_rResultant);
		}

		static float DistanceBetween(const _TVECTOR2& _krA, const _TVECTOR2& _krB)
		{
			float fXDifferenceSquared = (_krB.x - _krA.x) * (_krB.x - _krA.x);
			float fYDifferenceSquared = (_krB.y - _krA.y) * (_krB.y - _krA.y);

			return(sqrt(fXDifferenceSquared + fYDifferenceSquared));
		}

		static _TVECTOR2& Rotate(const _TVECTOR2& _krA, const float _fRotation, _TVECTOR2& _rResultant)
		{
			_rResultant.x = (_krA.x * cosf(_fRotation)) - (_krA.y * sinf(_fRotation));
			_rResultant.y = (_krA.y * cosf(_fRotation)) + (_krA.x * sinf(_fRotation));

			return(_rResultant);
		}

		/*
		*	VECTOR3 Functions
		*/
		static float Magnitude(const _TVECTOR3& _krA)
		{
			return(sqrt(pow(_krA.x,2) + pow(_krA.y,2) + pow(_krA.z,2)));
		}

		static float DotProduct(const _TVECTOR3& _krA, const _TVECTOR3& _krB)
		{
			return(_krA.x * _krB.x + _krA.y * _krB.y + _krA.z * _krB.z);
		}

		static _TVECTOR3& CrossProduct(const _TVECTOR3& _krA, const _TVECTOR3& _krB, 
									 _TVECTOR3& _rResultant)
		{
			_rResultant.x = _krA.y * _krB.z - _krA.z * _krB.y;
			_rResultant.y = _krA.z * _krB.x - _krA.x * _krB.z;
			_rResultant.z = _krA.x * _krB.y - _krA.y * _krB.x;
			return(_rResultant);
		}

		static _TVECTOR3& Normalise(const _TVECTOR3& _krA, _TVECTOR3& _rResultant)
		{
			float fMagnitudeOfA = Magnitude(_krA);

			_rResultant.x = _krA.x / fMagnitudeOfA;
			_rResultant.y = _krA.y / fMagnitudeOfA;
			_rResultant.z = _krA.z / fMagnitudeOfA;
			return(_rResultant);
		}

		// This function projects _krA onto _krB.
		static _TVECTOR3& Projection(const _TVECTOR3& _krA, const _TVECTOR3& _krB, 
								   _TVECTOR3& _rResultant)
		{
			// Function projects _krA onto _krB.
			float fProjectionScalar = DotProduct(_krA, _krB) / DotProduct(_krB, _krB);

			_rResultant.x = fProjectionScalar * _krB.x;
			_rResultant.y = fProjectionScalar * _krB.y;
			_rResultant.z = fProjectionScalar * _krB.z;

			return(_rResultant);
		}

		static float DistanceBetween(const _TVECTOR3& _krA, const _TVECTOR3& _krB)
		{
			float fXDifferenceSquared = (_krB.x - _krA.x) * (_krB.x - _krA.x);
			float fYDifferenceSquared = (_krB.y - _krA.y) * (_krB.y - _krA.y);
			float fZDifferenceSquared = (_krB.z - _krA.z) * (_krB.x - _krA.z);

			return(sqrt(fXDifferenceSquared + fYDifferenceSquared + fZDifferenceSquared));
		}
	}
}
#endif // __PAPYRUS_MATH_VECTORMATH_H__