/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#pragma once

#include "LocusCommonAPI.h"
#include "StaticAssertFalse.h"

#include <complex>

#include <cmath>

namespace Locus
{

/// Contains comparison functions taking floating point tolerance into account.
namespace Float
{

/// The tolerance to be used for float comparisons before being multiplied by the tolerance factor. It is 1e-4f.
LOCUS_COMMON_API extern const float FLOAT_BASE_TOLERANCE;

/// The tolerance to be used for double comparisons before being multiplied by the tolerance factor. It is 1e-7.
LOCUS_COMMON_API extern const double DOUBLE_BASE_TOLERANCE;

/// Tolerance factor used when an exact comparison is desired.
LOCUS_COMMON_API extern const float NO_TOLERANCE;

/// Tolerance factor used when the base tolerance is desired.
LOCUS_COMMON_API extern const float DEFAULT_TOLERANCE;

/*!
 * \brief Returns the tolerance value used for comparisons using
 * the template type (either float, double, or long double).
 *
 * \return toleranceFactor multiplied by the base tolerance of
 * the given type.
 */
template <typename T>
inline T Tolerance(T toleranceFactor)
{
   static_assert(StaticAssertFalse<T>::value, "Tolerance<T> called with unsupported type. T must be float, double, or long double");
}

template <>
inline float Tolerance(float toleranceFactor)
{
   return toleranceFactor * FLOAT_BASE_TOLERANCE;
}

template <>
inline double Tolerance(double toleranceFactor)
{
   return toleranceFactor * DOUBLE_BASE_TOLERANCE;
}

template <>
inline long double Tolerance(long double toleranceFactor)
{
   return toleranceFactor * DOUBLE_BASE_TOLERANCE;
}

////////////////////////////////////////////////////////

/// Returns true if x and y are within Tolerance<T> of each other.
template <typename T>
inline bool Equal(T x, T y, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return std::fabs(x - y) <= Tolerance<T>(toleranceFactor);
}

/// Returns true if x and y are NOT within Tolerance<T> of each other.
template <typename T>
inline bool NotEqual(T x, T y, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return std::fabs(x - y) > Tolerance<T>(toleranceFactor);
}

/// Returns true if x is within Tolerance<T> of zero.
template <typename T>
inline bool IsZero(T a, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return Equal<T>(a, T(), toleranceFactor);
}

/// Returns true if x is NOT within Tolerance<T> of zero.
template <typename T>
inline bool NotZero(T a, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return NotEqual<T>(a, T(), toleranceFactor);
}

/// Returns true if x is greater than y plus Tolerance<T>.
template <typename T>
inline bool Greater(T x, T y, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return x > (y + Tolerance<T>(toleranceFactor));
}

/// Returns true if x is less than y minus Tolerance<T>.
template <typename T>
inline bool Less(T x, T y, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return x < (y - Tolerance<T>(toleranceFactor));
}

/// Returns true if x is greater than y or if x and y are within Tolerance<T> of each other.
template <typename T>
inline bool GreaterOrEqual(T x, T y, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return (x > y) || Equal<T>(x, y, toleranceFactor);
}

/// Returns true if x is less than y or if x and y are within Tolerance<T> of each other.
template <typename T>
inline bool LessOrEqual(T x, T y, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return (x < y) || Equal<T>(x, y, toleranceFactor);
}

} // namespace Float

} // namespace Locus