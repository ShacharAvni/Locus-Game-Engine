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

#include "LocusGeometryAPI.h"

#include "Locus/Math/Vectors.h"

#include <functional>

namespace Locus
{

namespace Vec2D
{

LOCUS_GEOMETRY_API const FVector2& ZeroVector();
LOCUS_GEOMETRY_API const FVector2& XAxis();

}

LOCUS_GEOMETRY_API float DistanceBetween(const FVector2& v1, const FVector2& v2);
LOCUS_GEOMETRY_API float AngleBetweenRadians(const FVector2& v1, const FVector2& v2);
LOCUS_GEOMETRY_API float AngleBetweenDegrees(const FVector2& v1, const FVector2& v2);
LOCUS_GEOMETRY_API float AngleWithXAxisRadians(float x, float y);
LOCUS_GEOMETRY_API float AngleWithXAxisRadians(const FVector2& v);
LOCUS_GEOMETRY_API float Norm(const FVector2& v);
LOCUS_GEOMETRY_API float SquaredNorm(const FVector2& v);
LOCUS_GEOMETRY_API FVector2 NormVector(const FVector2& v);
LOCUS_GEOMETRY_API float Normalize(FVector2& v);
LOCUS_GEOMETRY_API float Dot(const FVector2& v1, const FVector2& v2);
LOCUS_GEOMETRY_API bool OrthogonalVectors(const FVector2& v1, const FVector2& v2);
LOCUS_GEOMETRY_API FVector3 Cross(const FVector2& v1, const FVector3& v2);
LOCUS_GEOMETRY_API bool GoTheSameWay(const FVector2& v1, const FVector2& v2);
LOCUS_GEOMETRY_API bool GoExactlyTheSameWay(const FVector2& v1, const FVector2& v2);

}

namespace std
{

template <> struct hash<Locus::FVector2>
{
   std::size_t operator()(const Locus::FVector2& vector2D) const
   {
      return hash<float>()(vector2D.x) ^ hash<float>()(vector2D.y);
   }
};

}