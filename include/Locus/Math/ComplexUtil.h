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

#include "Locus/Common/Float.h"

#include <complex>

namespace Locus
{

/// \return the real part of the given complex number.
template <typename T>
inline T& RealPart(std::complex<T>& x)
{
   return reinterpret_cast<T(&)[2]>(x)[0];
}

/// \return the imaginary part of the given complex number.
template <typename T>
inline T& ImaginaryPart(std::complex<T>& x)
{
   return reinterpret_cast<T(&)[2]>(x)[1];
}

/*!
 * \return true if the imaginary part of the given complex number is
 * within Tolerance<T> of zero.
 *
 * \sa FIsZero
 */
template <typename T>
inline bool IsReal(const std::complex<T>& x, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return FIsZero<T>(x.imag(), toleranceFactor);
}

/*!
 * \return true if the real parts and the imaginary parts of the
 * given complex numbers are within Tolerance<T> of each other.
 *
 * \sa FEqual
 */
template <typename T>
inline bool Equal(const std::complex<T>& x, const std::complex<T>& y, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return ( FEqual<T>(x.real(), y.real(), toleranceFactor) && FEqual<T>(x.imag(), y.imag(), toleranceFactor) );
}

/*!
 * \return true if the real parts or the imaginary parts of the
 * given complex numbers are NOT within Tolerance<T> of each other.
 *
 * \sa FNotEqual
 */
template <typename T>
inline bool NotEqual(const std::complex<T>& x, const std::complex<T>& y, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return ( FNotEqual<T>(x.real(), y.real(), toleranceFactor) || FNotEqual<T>(x.imag(), y.imag(), toleranceFactor) );
}

/*!
 * \return true if both the real part and the imaginary part of the
 * given complex number are within Tolerance<T> of zero.
 *
 * \sa FIsZero
 */
template <typename T>
inline bool IsZero(const std::complex<T>& x, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return ( FIsZero<T>(x.real(), toleranceFactor) && FIsZero<T>(x.imag(), toleranceFactor) );
}

/*!
 * \return true if the real part or the imaginary part of the
 * given complex number is NOT within Tolerance<T> of zero.
 *
 * \sa FNotZero
 */
template <typename T>
inline bool NotZero(const std::complex<T>& x, T toleranceFactor = DEFAULT_TOLERANCE)
{
   return ( FNotZero<T>(x.real(), toleranceFactor) || FNotZero<T>(x.imag(), toleranceFactor) );
}

}