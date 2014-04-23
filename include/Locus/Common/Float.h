 /********************************************************************************************************************************************************\
 *                                                                                                                                                        *
 *   This file is part of the Locus Game Engine                                                                                                           *
 *                                                                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                *
 *                                                                                                                                                        *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),   *
 *   to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the   *
 *   Software is furnished to do so, subject to the following conditions:                                                                                 *
 *                                                                                                                                                        *
 *   a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                      *
 *   b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the             *
 *      documentation and/or other materials provided with the distribution.                                                                              *
 *                                                                                                                                                        *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    *
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,            *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF            *
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,    *
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                   *
 *                                                                                                                                                        *
 \********************************************************************************************************************************************************/

#pragma once

#include "LocusCommonAPI.h"
#include "StaticAssertFalse.h"

#include <complex>

#include <cmath>

namespace Locus
{

namespace Float
{

LOCUS_COMMON_API extern const float FLOAT_BASE_TOLERANCE;
LOCUS_COMMON_API extern const double DOUBLE_BASE_TOLERANCE;

LOCUS_COMMON_API extern const float NO_TOLERANCE;
LOCUS_COMMON_API extern const float STANDARD_TOLERANCE;

template <typename T>
inline T Tolerance(T toleranceFactor = 1)
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

template <typename T>
inline bool FEqual(T x, T y, T toleranceFactor = 1)
{
   return std::fabs(x - y) <= Tolerance<T>(toleranceFactor);
}

template <typename T>
inline bool FNotEqual(T x, T y, T toleranceFactor = 1)
{
   return std::fabs(x - y) > Tolerance<T>(toleranceFactor);
}

template <typename T>
inline bool FIsZero(T a)
{
   return ( FEqual<T>(a, T()) );
}

template <typename T>
inline bool FNotZero(T a)
{
   return ( FNotEqual<T>(a, T()) );
}

template <typename T>
inline bool FGreater(T x, T y, T toleranceFactor = 1)
{
   return x > (y + Tolerance<T>(toleranceFactor));
}

template <typename T>
inline bool FLess(T x, T y, T toleranceFactor = 1)
{
   return x < (y - Tolerance<T>(toleranceFactor));
}

template <typename T>
inline bool FGreaterOrEqual(T x, T y, T toleranceFactor = 1)
{
   return (x > y) || FEqual<T>(x, y, toleranceFactor);
}

template <typename T>
inline bool FLessOrEqual(T x, T y, T toleranceFactor = 1)
{
   return (x < y) || FEqual<T>(x, y, toleranceFactor);
}

//return true if a <= b <= c
template <typename T>
inline bool FSuccessive(T a, T b, T c, T toleranceFactor = 1)
{
   return ( FGreaterOrEqual<T>(b, a, toleranceFactor) && FLessOrEqual<T>(b, c, toleranceFactor) );
}

template <typename T>
inline T FMin(T a, T b, T toleranceFactor = 1)
{
   return (FLess<T>(a, b, toleranceFactor) ? a : b);
}

template <typename T>
inline T FMax(T a, T b, T toleranceFactor = 1)
{
   return (FGreater<T>(a, b, toleranceFactor) ? a : b);
}

////////////////////////////////////////////////////////

template <typename T>
inline bool Equal(T x, T y, T /*toleranceFactor*/ = 1)
{
   return x == y;
}

template <typename T>
inline bool NotEqual(T x, T y, T /*toleranceFactor*/ = 1)
{
   return x != y;
}

template <typename T>
inline bool IsZero(T x)
{
   return x == T();
}

template <typename T>
inline bool NotZero(T x)
{
   return x != T();
}

template <typename T>
inline bool Greater(T x, T y, T /*toleranceFactor*/ = 1)
{
   return x > y;
}

template <typename T>
inline bool Less(T x, T y, T /*toleranceFactor*/ = 1)
{
   return x < y;
}

template <typename T>
inline bool GreaterOrEqual(T x, T y, T /*toleranceFactor*/ = 1)
{
   return x >= y;
}

template <typename T>
inline bool LessOrEqual(T x, T y, T /*toleranceFactor*/ = 1)
{
   return x <= y;
}

template <typename T>
inline bool Successive(T a, T b, T c, T /*toleranceFactor*/ = 1)
{
   return (b >= a) && (b <= c);
}

template <typename T>
inline T Min(T a, T b, T /*toleranceFactor*/ = 1)
{
   return a < b ? a : b;
}

template <typename T>
inline T Max(T a, T b, T /*toleranceFactor*/ = 1)
{
   return a > b ? a : b;
}

////////////////////////////////////////////////////////

template <typename T>
inline bool Equal(const std::complex<T>& x, const std::complex<T>& y, T toleranceFactor = 1)
{
   return ( FEqual<T>(x.real(), y.real(), toleranceFactor) && FEqual<T>(x.imag(), y.imag(), toleranceFactor) );
}

template <typename T>
inline bool NotEqual(const std::complex<T>& x, const std::complex<T>& y, T toleranceFactor = 1)
{
   return ( FNotEqual<T>(x.real(), y.real(), toleranceFactor) || FNotEqual<T>(x.imag(), y.imag(), toleranceFactor) );
}

template <typename T>
inline bool IsZero(const std::complex<T>& x)
{
   return ( FIsZero<T>(x.real()) && FIsZero<T>(x.imag()) );
}

template <typename T>
inline bool NotZero(const std::complex<T>& x)
{
   return ( FNotZero<T>(x.real()) || FNotZero<T>(x.imag()) );
}

//
// Unfortunately, due to a Visual Studio bug, when calling
// the following specializations, the default argument, toleranceFactor
// must be provided explicitly
//

////////////////////////////////// Float Specializations //////////////////////////////////

template <>
inline bool Equal(float x, float y, float toleranceFactor)
{
   return FEqual<float>(x, y, toleranceFactor);
}

template <>
inline bool NotEqual(float x, float y, float toleranceFactor)
{
   return FNotEqual<float>(x, y, toleranceFactor);
}

template <>
inline bool IsZero(float x)
{
   return FIsZero<float>(x);
}

template <>
inline bool NotZero(float x)
{
   return FNotZero<float>(x);
}

template <>
inline bool Greater(float x, float y, float toleranceFactor)
{
   return FGreater<float>(x, y, toleranceFactor);
}

template <>
inline bool Less(float x, float y, float toleranceFactor)
{
   return FLess<float>(x, y, toleranceFactor);
}

template <>
inline bool GreaterOrEqual(float x, float y, float toleranceFactor)
{
   return FGreaterOrEqual<float>(x, y, toleranceFactor);
}

template <>
inline bool LessOrEqual(float x, float y, float toleranceFactor)
{
   return FLessOrEqual<float>(x, y, toleranceFactor);
}

template <>
inline bool Successive(float a, float b, float c, float toleranceFactor)
{
   return FSuccessive<float>(a, b, c, toleranceFactor);
}

template <>
inline float Min(float a, float b, float toleranceFactor)
{
   return FMin<float>(a, b, toleranceFactor);
}

template <>
inline float Max(float a, float b, float toleranceFactor)
{
   return FMax<float>(a, b, toleranceFactor);
}

////////////////////////////////// Double Specializations //////////////////////////////////

template <>
inline bool Equal(double x, double y, double toleranceFactor)
{
   return FEqual<double>(x, y, toleranceFactor);
}

template <>
inline bool NotEqual(double x, double y, double toleranceFactor)
{
   return FNotEqual<double>(x, y, toleranceFactor);
}

template <>
inline bool IsZero(double x)
{
   return FIsZero<double>(x);
}

template <>
inline bool NotZero(double x)
{
   return FNotZero<double>(x);
}

template <>
inline bool Greater(double x, double y, double toleranceFactor)
{
   return FGreater<double>(x, y, toleranceFactor);
}

template <>
inline bool Less(double x, double y, double toleranceFactor)
{
   return FLess<double>(x, y, toleranceFactor);
}

template <>
inline bool GreaterOrEqual(double x, double y, double toleranceFactor)
{
   return FGreaterOrEqual<double>(x, y, toleranceFactor);
}

template <>
inline bool LessOrEqual(double x, double y, double toleranceFactor)
{
   return FLessOrEqual<double>(x, y, toleranceFactor);
}

template <>
inline bool Successive(double a, double b, double c, double toleranceFactor)
{
   return FSuccessive<double>(a, b, c, toleranceFactor);
}

template <>
inline double Min(double a, double b, double toleranceFactor)
{
   return FMin<double>(a, b, toleranceFactor);
}

template <>
inline double Max(double a, double b, double toleranceFactor)
{
   return FMax<double>(a, b, toleranceFactor);
}

////////////////////////////////// Long Double Specializations //////////////////////////////////

template <>
inline bool Equal(long double x, long double y, long double toleranceFactor)
{
   return FEqual<long double>(x, y, toleranceFactor);
}

template <>
inline bool NotEqual(long double x, long double y, long double toleranceFactor)
{
   return FNotEqual<long double>(x, y, toleranceFactor);
}

template <>
inline bool IsZero(long double x)
{
   return FIsZero<long double>(x);
}

template <>
inline bool NotZero(long double x)
{
   return FNotZero<long double>(x);
}

template <>
inline bool Greater(long double x, long double y, long double toleranceFactor)
{
   return FGreater<long double>(x, y, toleranceFactor);
}

template <>
inline bool Less(long double x, long double y, long double toleranceFactor)
{
   return FLess<long double>(x, y, toleranceFactor);
}

template <>
inline bool GreaterOrEqual(long double x, long double y, long double toleranceFactor)
{
   return FGreaterOrEqual<long double>(x, y, toleranceFactor);
}

template <>
inline bool LessOrEqual(long double x, long double y, long double toleranceFactor)
{
   return FLessOrEqual<long double>(x, y, toleranceFactor);
}

template <>
inline bool Successive(long double a, long double b, long double c, long double toleranceFactor)
{
   return FSuccessive<long double>(a, b, c, toleranceFactor);
}

template <>
inline long double Min(long double a, long double b, long double toleranceFactor)
{
   return FMin<long double>(a, b, toleranceFactor);
}

template <>
inline long double Max(long double a, long double b, long double toleranceFactor)
{
   return FMax<long double>(a, b, toleranceFactor);
}

}

}