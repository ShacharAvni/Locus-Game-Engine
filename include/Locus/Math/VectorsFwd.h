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

#include "LocusMathAPI.h"

namespace Locus
{

template <typename ElementType>
struct LOCUS_MATH_API Vector2;

template <typename ElementType>
struct LOCUS_MATH_API Vector3;

template <typename ElementType>
struct LOCUS_MATH_API Vector4;

typedef Vector2<float> FVector2;
typedef Vector2<int> IVector2;

typedef Vector3<float> FVector3;
typedef Vector3<int> IVector3;

typedef Vector4<float> FVector4;
typedef Vector4<int> IVector4;

}