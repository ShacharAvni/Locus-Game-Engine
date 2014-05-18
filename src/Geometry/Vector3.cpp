/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Vector3.h"
#include "Locus/Geometry/Vector2.h"
#include "Locus/Geometry/Geometry.h"
#include "Locus/Geometry/Quaternion.h"

#include "Locus/Common/Float.h"

#include <assert.h>

namespace Locus
{

Vector3::Vector3()
   : x(0), y(0), z(0)
{
}

Vector3::Vector3(float x, float y, float z)
   : x(x), y(y), z(z)
{
}

Vector3::Vector3(const std::vector<float>& elements)
{
   assert(elements.size() == 3);

   x = elements[0];
   y = elements[1];
   z = elements[2];
}

Vector3::Vector3(std::initializer_list<float> elements)
{
   assert(elements.size() == 3);

   x = *(elements.begin());
   y = *(elements.begin() + 1);
   z = *(elements.begin() + 2);
}

Vector3::Vector3(const Vector2& v2D)
   : x(v2D.x), y(v2D.y), z(0.0f)
{
}

const Vector3& Vector3::ZeroVector()
{
   static Vector3 zeroVector(0.0f, 0.0f, 0.0f);
   return zeroVector;
}

const Vector3& Vector3::XAxis()
{
   static Vector3 xAxis(1.0f, 0.0f, 0.0f);
   return xAxis;
}

const Vector3& Vector3::YAxis()
{
   static Vector3 yAxis(0.0f, 1.0f, 0.0f);
   return yAxis;
}

const Vector3& Vector3::ZAxis()
{
   static Vector3 zAxis(0.0f, 0.0f, 1.0f);
   return zAxis;
}

const Vector3& Vector3::NegativeXAxis()
{
   static Vector3 negativeXAxis(-1.0f, 0.0f, 0.0f);
   return negativeXAxis;
}

const Vector3& Vector3::NegativeYAxis()
{
   static Vector3 negativeYAxis(0.0f, -1.0f, 0.0f);
   return negativeYAxis;
}

const Vector3& Vector3::NegativeZAxis()
{
   static Vector3 negativeZAxis(0.0f, 0.0f, -1.0f);
   return negativeZAxis;
}

const Vector3& Vector3::Diagonal()
{
   static Vector3 diagonal(1.0f, 1.0f, 1.0f);
   return diagonal;
}

const Vector3& Vector3::DiagonalNormalized()
{
   #define ROOT_THREE 1.7320508075688773f

   static Vector3 diagonalNormalized(ROOT_THREE, ROOT_THREE, ROOT_THREE);

   #undef ROOT_THREE

   return diagonalNormalized;
}

Vector3& Vector3::operator=(const Vector2& v2D)
{
   x = v2D.x;
   y = v2D.y;

   return *this;
}

void Vector3::set(float x, float y, float z)
{
   this->x = x;
   this->y = y;
   this->z = z;
}

float Vector3::distanceTo(const Vector3& v) const
{
   return sqrt( (v.x - x)*(v.x - x) + (v.y - y)*(v.y - y) + (v.z - z)*(v.z - z) );
}

float Vector3::angleBetweenRadians(const Vector3& v) const
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

float Vector3::angleBetweenDegrees(const Vector3& v) const
{
   return angleBetweenRadians(v) * TO_DEGREES;
}

float Vector3::norm() const
{
   return sqrt(x*x + y*y + z*z);
}

float Vector3::squaredNorm() const
{
   return x*x + y*y + z*z;
}

Vector3 Vector3::normVector() const
{
   float length = norm();

   if (length != 0.0f)
   {
      return Vector3(x/length, y/length, z/length);
   }
   else
   {
      return *this;
   }
}

void Vector3::normalize()
{
   float length = norm();

   if (length != 0.0f)
   {
      x /= length;
      y /= length;
      z /= length;
   }
}

float Vector3::dot(const Vector3& v) const
{
   return( (x * v.x) + (y * v.y) + (z * v.z) );
}

bool Vector3::isOrthogonalTo(const Vector3& v) const
{
   return Float::FIsZero<float>( abs(dot(v)) );
}

Vector3 Vector3::cross(const Vector3& v) const
{
   return Vector3((y * v.z) - (z * v.y),
                     (z * v.x) - (x * v.z),
                     (x * v.y) - (y * v.x));
}

void Vector3::rotateAround(const Vector3& axis, float angleRadians)
{
   Quaternion q(axis, angleRadians);
   Quaternion v(0, x, y, z);

   Quaternion result = q * v * q.Reciprocal();

   x = result.b;
   y = result.c;
   z = result.d;
}

void Vector3::rotate(float theta, const Vector3& a)
{
   rotateAround(a, TO_RADIANS * theta);
}

void Vector3::rotateRadians(float theta, const Vector3& a)
{
   rotateAround(a, theta);
}

bool Vector3::goesTheSameWayAs(const Vector3& other) const
{
   //i.e. angle between this and other is less than 90 degrees
   return dot(other) > 0.0f;
}

bool Vector3::goesExactlyTheSameWayAs(const Vector3& other) const
{
   return ( (cross(other) == Vector3::ZeroVector()) && goesTheSameWayAs(other) );
}

bool Vector3::ApproximatelyEqualTo(const Vector3& other, float toleranceFactor) const
{
   return ( Float::FEqual<float>(x, other.x, toleranceFactor) && Float::FEqual<float>(y, other.y, toleranceFactor) && Float::FEqual<float>(z, other.z, toleranceFactor) );
}

bool Vector3::PreciselyEqualTo(const Vector3& other) const
{
   return ( (x == other.x) && (y == other.y) && (z == other.z) );
}

//////////////////////////////////////Operators//////////////////////////////////////////

bool operator==(const Vector3& v1, const Vector3& v2)
{
   return v1.ApproximatelyEqualTo(v2);
}

bool operator!=(const Vector3& v1, const Vector3& v2)
{
   return !(v1 == v2);
}

bool operator <(const Vector3& v1, const Vector3& v2)
{
   if (v1.x == v2.x)
   {
      if (v1.y == v2.y)
      {
         if (v1.z == v2.z)
         {
            return false;
         }
         else
         {
            return v1.z < v2.z;
         }
      }
      else
      {
         return v1.y < v2.y;
      }
   }
   else
   {
      return v1.x < v2.x;
   }
}

Vector3& operator*=(Vector3& v, float s)
{
   v = v * s;
   return v;
}

Vector3& operator/=(Vector3& v, float d)
{
   v = v / d;
   return v;
}

Vector3& operator+=(Vector3& v1, const Vector3& v2)
{
   v1.x += v2.x;
   v1.y += v2.y;
   v1.z += v2.z;

   return v1;
}

Vector3& operator-=(Vector3& v1, const Vector3& v2)
{
   v1.x -= v2.x;
   v1.y -= v2.y;
   v1.z -= v2.z;

   return v1;
}

Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
   return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
   return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 operator*(const Vector3& v, float s)
{
   return Vector3(v.x * s, v.y * s, v.z * s);
}

Vector3 operator*(float s, const Vector3& v)
{
   return Vector3(v.x * s, v.y * s, v.z * s);
}

Vector3 operator/(const Vector3& v, float d)
{
   return Vector3(v.x / d, v.y / d, v.z / d);
}

Vector3 operator-(const Vector3& v)
{
   return Vector3(-v.x, -v.y, -v.z);
}

}