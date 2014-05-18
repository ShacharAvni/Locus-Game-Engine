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

template <typename T>
T& RealPart(std::complex<T>& x)
{
   return reinterpret_cast<T(&)[2]>(x)[0];
}

template <typename T>
T& ImaginaryPart(std::complex<T>& x)
{
   return reinterpret_cast<T(&)[2]>(x)[1];
}

template <typename T>
bool IsReal(const std::complex<T>& x)
{
   return Float::FIsZero<T>(x.imag());
}

}