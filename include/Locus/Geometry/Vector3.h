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

#include <vector>
#include <functional>
#include <initializer_list>

namespace Locus
{

class Vector2;

class LOCUS_GEOMETRY_API Vector3
{
public:
   Vector3();
   Vector3(float x, float y, float z);
   Vector3(const std::vector<float>& elements);
   Vector3(std::initializer_list<float> elements);
   Vector3(const Vector2& v2D);

   enum Coordinate
   {
      Coordinate_X = 0,
      Coordinate_Y = 1,
      Coordinate_Z = 2
   };

   float x;
   float y;
   float z;

   static const Vector3& ZeroVector();
   static const Vector3& XAxis();
   static const Vector3& YAxis();
   static const Vector3& ZAxis();
   static const Vector3& NegativeXAxis();
   static const Vector3& NegativeYAxis();
   static const Vector3& NegativeZAxis();
   static const Vector3& Diagonal();
   static const Vector3& DiagonalNormalized();

   inline float& operator[](Coordinate d)
   {
      switch(d)
      {
         case Coordinate_X:
            return x;

         case Coordinate_Y:
            return y;

         case Coordinate_Z:
         default:
            return z;
      }
   }

   inline const float& operator[](Coordinate d) const
   {
      return const_cast<Vector3*>(this)->operator[](d);
   }

   Vector3& operator=(const Vector2& v2D);

   void set(float x, float y, float z);

   float distanceTo(const Vector3& v) const;
   float angleBetweenRadians(const Vector3& v) const;
   float angleBetweenDegrees(const Vector3& v) const;

   //vector functions
   float norm() const;
   float squaredNorm() const;
   Vector3 normVector() const;
   void normalize();
   float dot(const Vector3& v) const;
   bool isOrthogonalTo(const Vector3& v) const;
   Vector3 cross(const Vector3& v) const;
   void rotateAround(const Vector3& axis, float angleRadians);
   void rotate(float theta, const Vector3& a);
   void rotateRadians(float theta, const Vector3& a);

   bool goesTheSameWayAs(const Vector3& other) const;
   bool goesExactlyTheSameWayAs(const Vector3& other) const;
   bool ApproximatelyEqualTo(const Vector3& other, float toleranceFactor = 1) const;
   bool PreciselyEqualTo(const Vector3& other) const;
};

LOCUS_GEOMETRY_API Vector3 operator+(const Vector3& v1, const Vector3& v2);
LOCUS_GEOMETRY_API Vector3 operator-(const Vector3& v1, const Vector3& v2);
LOCUS_GEOMETRY_API Vector3 operator*(const Vector3& v, float s);
LOCUS_GEOMETRY_API Vector3 operator*(float s, const Vector3& v);
LOCUS_GEOMETRY_API Vector3 operator/(const Vector3& v, float d);
LOCUS_GEOMETRY_API Vector3 operator-(const Vector3& v);

LOCUS_GEOMETRY_API bool operator==(const Vector3& v1, const Vector3& v2);
LOCUS_GEOMETRY_API bool operator!=(const Vector3& v1, const Vector3& v2);
LOCUS_GEOMETRY_API Vector3& operator+=(Vector3& v1, const Vector3& v2);
LOCUS_GEOMETRY_API Vector3& operator-=(Vector3& v1, const Vector3& v2);
LOCUS_GEOMETRY_API Vector3& operator*=(Vector3& v, float s);
LOCUS_GEOMETRY_API Vector3& operator/=(Vector3& v, float d);

//for sorting within the Model class
LOCUS_GEOMETRY_API bool operator <(const Vector3& v1, const Vector3& v2);

}

namespace std
{

template <> struct hash<Locus::Vector3>
{
   std::size_t operator()(const Locus::Vector3& vec3D) const
   {
      return hash<float>()(vec3D.x) ^ hash<float>()(vec3D.y) ^ hash<float>()(vec3D.z);
   }
};

}