/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Vector2Geometry.h"
#include "Locus/Geometry/Vector3Geometry.h"
#include "Locus/Geometry/Geometry.h"

#include "Locus/Common/Float.h"

#include <cmath>

namespace Locus
{

namespace Vec2D
{

const FVector2& ZeroVector()
{
   static FVector2 zeroVector(0.0f, 0.0f);
   return zeroVector;
}

const FVector2& XAxis()
{
   static FVector2 xAxis(1.0f, 0.0f);
   return xAxis;
}

}

float DistanceBetween(const FVector2& v1, const FVector2& v2)
{
   return std::sqrt( (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y) );
}

float AngleBetweenRadians(const FVector2& v1, const FVector2& v2)
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

float AngleBetweenDegrees(const FVector2& v1, const FVector2& v2)
{
   return AngleBetweenRadians(v1, v2) * TO_DEGREES;
}

float AngleWithXAxisRadians(float x, float y)
{
   float angle = 0.0f;
      
   if (FEqual<float>(x, 0.0f))
   {
      if (FEqual<float>(y, 0.0f))
      {
         angle = 0.0f;
      }
      else if (FGreater<float>(y, 0.0f))
      {
         angle = PI/2;
      }
      else
      {
         angle = (3 * PI) / 2;
      }
   }
   else
   {
      angle = std::atan(abs(y/x));

      if (FLessOrEqual<float>(x, 0.0f) && FGreaterOrEqual<float>(y, 0.0f))
      {
         //second quadrant
         angle = PI - angle;
      }
      else if (FLessOrEqual<float>(x, 0.0f) && FLess<float>(y, 0.0f))
      {
         //third quadrant
         angle = PI + angle;
      }
      else if (FGreater<float>(x, 0.0f)  && FLess<float>(y, 0.0f))
      {
         //fourth quadrant
         angle = 2 * PI - angle;
      }
   }

   return angle;
}

float AngleWithXAxisRadians(const FVector2& v)
{
   return AngleWithXAxisRadians(v.x, v.y);
}

float Norm(const FVector2& v)
{
   return std::sqrt(v.x*v.x + v.y*v.y);
}

float SquaredNorm(const FVector2& v)
{
   return (v.x*v.x + v.y*v.y);
}

FVector2 NormVector(const FVector2& v)
{
   float length = Norm(v);

   if (length != 0.0f)
   {
      return FVector2(v.x/length, v.y/length);
   }

   return v;
}

void Normalize(FVector2& v)
{
   float length = Norm(v);

   if (length != 0.0f)
   {
      v.x /= length;
      v.y /= length;
   }
}

float Dot(const FVector2& v1, const FVector2& v2)
{
   return( (v1.x * v2.x) + (v1.y * v2.y) );
}

bool OrthogonalVectors(const FVector2& v1, const FVector2& v2)
{
   return FIsZero<float>( std::abs(Dot(v1, v2)) );
}

FVector3 Cross(const FVector2& v1, const FVector3& v2)
{
   return FVector3( 0.0f, 0.0f, (v1.x * v2.y) - (v1.y * v2.x) );
}

bool GoTheSameWay(const FVector2& v1, const FVector2& v2)
{
   //i.e. angle between this and other is less than 90 degrees
   return FGreater<float>(Dot(v1, v2), 0.0f);
}

bool GoExactlyTheSameWay(const FVector2& v1, const FVector2& v2)
{
   return ( ApproximatelyEqual(Cross(v1, v2), Vec3D::ZeroVector()) && GoTheSameWay(v1, v2) );
}

}