/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Vector3Geometry.h"
#include "Locus/Geometry/Geometry.h"
#include "Locus/Geometry/Quaternion.h"

#include "Locus/Common/Float.h"

#include <cmath>

namespace Locus
{

namespace Vec3D
{

const FVector3& ZeroVector()
{
   static FVector3 zeroVector(0.0f, 0.0f, 0.0f);
   return zeroVector;
}

const FVector3& XAxis()
{
   static FVector3 xAxis(1.0f, 0.0f, 0.0f);
   return xAxis;
}

const FVector3& YAxis()
{
   static FVector3 yAxis(0.0f, 1.0f, 0.0f);
   return yAxis;
}

const FVector3& ZAxis()
{
   static FVector3 zAxis(0.0f, 0.0f, 1.0f);
   return zAxis;
}

const FVector3& NegativeXAxis()
{
   static FVector3 negativeXAxis(-1.0f, 0.0f, 0.0f);
   return negativeXAxis;
}

const FVector3& NegativeYAxis()
{
   static FVector3 negativeYAxis(0.0f, -1.0f, 0.0f);
   return negativeYAxis;
}

const FVector3& NegativeZAxis()
{
   static FVector3 negativeZAxis(0.0f, 0.0f, -1.0f);
   return negativeZAxis;
}

const FVector3& Diagonal()
{
   static FVector3 diagonal(1.0f, 1.0f, 1.0f);
   return diagonal;
}

const FVector3& DiagonalNormalized()
{
   #define ROOT_THREE 1.7320508075688773f

   static FVector3 diagonalNormalized(ROOT_THREE, ROOT_THREE, ROOT_THREE);

   #undef ROOT_THREE

   return diagonalNormalized;
}

}

float DistanceBetween(const FVector3& v1, const FVector3& v2)
{
   return std::sqrt( (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y) + (v1.z - v2.z)*(v1.z - v2.z) );
}

float AngleBetweenRadians(const FVector3& v1, const FVector3& v2)
{
   float denominator = Norm(v1) * Norm(v2);

   if (FNotZero<float>(denominator))
   {
      return std::acos( Dot(v1, v2) / denominator );
   }
   else
   {
      return 0.0f;
   }
}

float AngleBetweenDegrees(const FVector3& v1, const FVector3& v2)
{
   return AngleBetweenRadians(v1, v2) * TO_DEGREES;
}

float Norm(const FVector3& v)
{
   return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

float SquaredNorm(const FVector3& v)
{
   return (v.x*v.x + v.y*v.y + v.z*v.z);
}

FVector3 NormVector(const FVector3& v)
{
   float length = Norm(v);

   if (length != 0.0f)
   {
      return FVector3(v.x/length, v.y/length, v.z/length);
   }

   return v;
}

float Normalize(FVector3& v)
{
   float length = Norm(v);

   if (length != 0.0f)
   {
      v.x /= length;
      v.y /= length;
      v.z /= length;
   }

   return length;
}

float Dot(const FVector3& v1, const FVector3& v2)
{
   return( (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) );
}

bool OrthogonalVectors(const FVector3& v1, const FVector3& v2)
{
   return FIsZero<float>( std::abs(Dot(v1, v2)) );
}

FVector3 Cross(const FVector3& v1, const FVector3& v2)
{
   return FVector3((v1.y * v2.z) - (v1.z * v2.y),
                   (v1.z * v2.x) - (v1.x * v2.z),
                   (v1.x * v2.y) - (v1.y * v2.x));
}

void RotateAround(FVector3& v, const FVector3& axis, float angleRadians)
{
   Quaternion q(axis, angleRadians);
   Quaternion p(0, v.x, v.y, v.z);

   Quaternion result = q * p * q.Reciprocal();

   v.x = result.b;
   v.y = result.c;
   v.z = result.d;
}

void RotateAroundDegrees(FVector3& v, const FVector3& axis, float angleDegrees)
{
   RotateAround(v, axis, TO_RADIANS * angleDegrees);
}

bool GoTheSameWay(const FVector3& v1, const FVector3& v2)
{
   //i.e. angle between v1 and v2 is less than 90 degrees
   return (Dot(v1, v2) > 0.0f);
}

bool GoExactlyTheSameWay(const FVector3& v1, const FVector3& v2)
{
   return ( ApproximatelyEqual(Cross(v1, v2), Vec3D::ZeroVector()) && GoTheSameWay(v1, v2) );
}

}