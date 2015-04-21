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

namespace Vec3D
{

LOCUS_GEOMETRY_API const FVector3& ZeroVector();
LOCUS_GEOMETRY_API const FVector3& XAxis();
LOCUS_GEOMETRY_API const FVector3& YAxis();
LOCUS_GEOMETRY_API const FVector3& ZAxis();
LOCUS_GEOMETRY_API const FVector3& NegativeXAxis();
LOCUS_GEOMETRY_API const FVector3& NegativeYAxis();
LOCUS_GEOMETRY_API const FVector3& NegativeZAxis();
LOCUS_GEOMETRY_API const FVector3& Diagonal();
LOCUS_GEOMETRY_API const FVector3& DiagonalNormalized();

}

LOCUS_GEOMETRY_API float DistanceBetween(const FVector3& v1, const FVector3& v2);
LOCUS_GEOMETRY_API float AngleBetweenRadians(const FVector3& v1, const FVector3& v2);
LOCUS_GEOMETRY_API float AngleBetweenDegrees(const FVector3& v1, const FVector3& v2);
LOCUS_GEOMETRY_API float Norm(const FVector3& v);
LOCUS_GEOMETRY_API float SquaredNorm(const FVector3& v);
LOCUS_GEOMETRY_API FVector3 NormVector(const FVector3& v);
LOCUS_GEOMETRY_API float Normalize(FVector3& v);
LOCUS_GEOMETRY_API float Dot(const FVector3& v1, const FVector3& v2);
LOCUS_GEOMETRY_API bool OrthogonalVectors(const FVector3& v1, const FVector3& v2);
LOCUS_GEOMETRY_API FVector3 Cross(const FVector3& v1, const FVector3& v2);
LOCUS_GEOMETRY_API void RotateAround(FVector3& v, const FVector3& axis, float angleRadians);
LOCUS_GEOMETRY_API void RotateAroundDegrees(FVector3& v, const FVector3& axis, float angleDegrees);
LOCUS_GEOMETRY_API bool GoTheSameWay(const FVector3& v1, const FVector3& v2);
LOCUS_GEOMETRY_API bool GoExactlyTheSameWay(const FVector3& v1, const FVector3& v2);

}

namespace std
{

template <> struct hash<Locus::FVector3>
{
   std::size_t operator()(const Locus::FVector3& vector3D) const
   {
      return hash<float>()(vector3D.x) ^ hash<float>()(vector3D.y) ^ hash<float>()(vector3D.z);
   }
};

}