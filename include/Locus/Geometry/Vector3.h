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