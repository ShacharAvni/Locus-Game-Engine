/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Vector2.h"
#include "Locus/Geometry/Geometry.h"

#include "Locus/Common/Float.h"

#include <cassert>

namespace Locus
{

Vector2::Vector2()
   : x(0), y(0)
{
}

Vector2::Vector2(float x, float y)
   : x(x), y(y)
{
}

Vector2::Vector2(const std::vector<float>& elements)
{
   assert(elements.size() == 2);

   x = elements[0];
   y = elements[1];
}

Vector2::Vector2(std::initializer_list<float> elements)
{
   assert(elements.size() == 2);

   x = *(elements.begin());
   y = *(elements.begin() + 1);
}

const Vector2& Vector2::ZeroVector()
{
   static Vector2 zeroVector(0.0f, 0.0f);
   return zeroVector;
}

const Vector2& Vector2::XAxis()
{
   static Vector2 xAxis(1.0f, 0.0f);
   return xAxis;
}

void Vector2::set(float x, float y)
{
   this->x = x;
   this->y = y;
}

float Vector2::distanceTo(const Vector2& v) const
{
   return sqrt( (v.x - x)*(v.x - x) + (v.y - y)*(v.y - y) );
}

float Vector2::angleBetweenRadians(const Vector2& v) const
{
   float denominator = norm() * v.norm();

   if (Float::NotZero<float>(denominator))
   {
      return acos( dot(v) / denominator );
   }
   else
   {
      return 0.0f;
   }
}

float Vector2::angleBetweenDegrees(const Vector2& v) const
{
   return angleBetweenRadians(v) * TO_DEGREES;
}

float Vector2::AngleWithXAxisRadians(float x, float y)
{
   float angle = 0.0f;
      
   if (Float::Equal<float>(x, 0.0f))
   {
      if (Float::Equal<float>(y, 0.0f))
      {
         angle = 0.0f;
      }
      else if (Float::Greater<float>(y, 0.0f))
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
      angle = atan(abs(y/x));

      if (Float::LessOrEqual<float>(x, 0.0f) && Float::GreaterOrEqual<float>(y, 0.0f))
      {
         //second quadrant
         angle = PI - angle;
      }
      else if (Float::LessOrEqual<float>(x, 0.0f) && Float::Less<float>(y, 0.0f))
      {
         //third quadrant
         angle = PI + angle;
      }
      else if (Float::Greater<float>(x, 0.0f)  && Float::Less<float>(y, 0.0f))
      {
         //fourth quadrant
         angle = 2 * PI - angle;
      }
   }

   return angle;
}

float Vector2::AngleWithXAxisRadians() const
{
   return AngleWithXAxisRadians(x, y);
}

float Vector2::norm() const
{
   return sqrt(x*x + y*y);
}

float Vector2::squaredNorm() const
{
   return x*x + y*y;
}

Vector2 Vector2::normVector() const
{
   float length = norm();

   if (length != 0.0f)
   {
      return Vector2(x/length, y/length);
   }
   else
   {
      return *this;
   }
}

void Vector2::normalize()
{
   float length = norm();

   if (length != 0.0f)
   {
      x /= length;
      y /= length;
   }
}

float Vector2::dot(const Vector2& v) const
{
   return( (x * v.x) + (y * v.y) );
}

bool Vector2::isOrthogonalTo(const Vector2& v) const
{
   return Float::IsZero<float>( abs(dot(v)) );
}

Vector3 Vector2::cross(const Vector2& v) const
{
   return Vector3( 0.0f, 0.0f, (x * v.y) - (y * v.x) );
}

bool Vector2::goesTheSameWayAs(const Vector2& other) const
{
   //i.e. angle between this and other is less than 90 degrees
   return Float::Greater<float>(dot(other), 0.0f);
}

bool Vector2::goesExactlyTheSameWayAs(const Vector2& other) const
{
   return ( (cross(other) == Vector3::ZeroVector()) && goesTheSameWayAs(other) );
}

bool Vector2::ApproximatelyEqualTo(const Vector2& other, float toleranceFactor) const
{
   return ( Float::Equal<float>(x, other.x, toleranceFactor) && Float::Equal<float>(y, other.y, toleranceFactor) );
}

bool Vector2::PreciselyEqualTo(const Vector2& other) const
{
   return ( (x == other.x) && (y == other.y) );
}

//////////////////////////////////////Operators//////////////////////////////////////////

bool operator==(const Vector2& v1, const Vector2& v2)
{
   return v1.ApproximatelyEqualTo(v2);
}

bool operator!=(const Vector2& v1, const Vector2& v2)
{
   return !(v1 == v2);
}

Vector2& operator*=(Vector2& v, float s)
{
   v = v * s;
   return v;
}

Vector2& operator/=(Vector2& v, float d)
{
   v = v / d;
   return v;
}

Vector2& operator+=(Vector2& v1, const Vector2& v2)
{
   v1.x += v2.x;
   v1.y += v2.y;

   return v1;
}

Vector2& operator-=(Vector2& v1, const Vector2& v2)
{
   v1.x -= v2.x;
   v1.y -= v2.y;

   return v1;
}

Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
   return Vector2(v1.x + v2.x, v1.y + v2.y);
}

Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
   return Vector2(v1.x - v2.x, v1.y - v2.y);
}

Vector2 operator*(const Vector2& v, float s)
{
   return Vector2(v.x * s, v.y * s);
}

Vector2 operator*(float s, const Vector2& v)
{
   return Vector2(v.x * s, v.y * s);
}

Vector2 operator/(const Vector2& v, float d)
{
   return Vector2(v.x / d, v.y / d);
}

Vector2 operator-(const Vector2& v)
{
   return Vector2(-v.x, -v.y);
}

}