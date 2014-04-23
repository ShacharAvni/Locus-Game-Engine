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

#include "Locus/Geometry/Vector2.h"
#include "Locus/Geometry/Geometry.h"

#include "Locus/Common/Float.h"

#include <assert.h>

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

   if (Float::FNotZero<float>(denominator))
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
      
   if (Float::FEqual<float>(x, 0.0f))
   {
      if (Float::FEqual<float>(y, 0.0f))
      {
         angle = 0.0f;
      }
      else if (Float::FGreater<float>(y, 0.0f))
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

      if (Float::FLessOrEqual<float>(x, 0.0f) && Float::FGreaterOrEqual<float>(y, 0.0f))
      {
         //second quadrant
         angle = PI - angle;
      }
      else if (Float::FLessOrEqual<float>(x, 0.0f) && Float::FLess<float>(y, 0.0f))
      {
         //third quadrant
         angle = PI + angle;
      }
      else if (Float::FGreater<float>(x, 0.0f)  && Float::FLess<float>(y, 0.0f))
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
   return Float::FIsZero<float>( abs(dot(v)) );
}

Vector3 Vector2::cross(const Vector2& v) const
{
   return Vector3( 0.0f, 0.0f, (x * v.y) - (y * v.x) );
}

bool Vector2::goesTheSameWayAs(const Vector2& other) const
{
   //i.e. angle between this and other is less than 90 degrees
   return Float::FGreater<float>(dot(other), 0.0f);
}

bool Vector2::goesExactlyTheSameWayAs(const Vector2& other) const
{
   return ( (cross(other) == Vector3::ZeroVector()) && goesTheSameWayAs(other) );
}

bool Vector2::ApproximatelyEqualTo(const Vector2& other, float toleranceFactor) const
{
   return ( Float::FEqual<float>(x, other.x, toleranceFactor) && Float::FEqual<float>(y, other.y, toleranceFactor) );
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