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

#include "Vector3.h"

#include <vector>
#include <functional>
#include <initializer_list>

namespace Locus
{

class LOCUS_GEOMETRY_API Vector2
{
public:
   Vector2();
   Vector2(float x, float y);
   Vector2(const std::vector<float>& elements);
   Vector2(std::initializer_list<float> elements);

   enum Coordinate
   {
      Coordinate_X = 0,
      Coordinate_Y = 1
   };

   float x;
   float y;

   static const Vector2& ZeroVector();
   static const Vector2& XAxis();

   inline float& operator[](Coordinate d)
   {
      switch(d)
      {
         case Coordinate_X:
            return x;

         case Coordinate_Y:
         default:
            return y;
      }
   }

   inline const float& operator[](Coordinate d) const
   {
      return const_cast<Vector2*>(this)->operator[](d);
   }

   void set(float x, float y);

   float distanceTo(const Vector2& v) const;
   float angleBetweenRadians(const Vector2& v) const;
   float angleBetweenDegrees(const Vector2& v) const;

   static float AngleWithXAxisRadians(float x, float y);
   float AngleWithXAxisRadians() const;

   //vector functions
   float norm() const;
   float squaredNorm() const;
   Vector2 normVector() const;
   void normalize();
   float dot(const Vector2& v) const;
   bool isOrthogonalTo(const Vector2& v) const;
   Vector3 cross(const Vector2& v) const;
   bool goesTheSameWayAs(const Vector2& other) const;
   bool goesExactlyTheSameWayAs(const Vector2& other) const;
   bool ApproximatelyEqualTo(const Vector2& other, float toleranceFactor = 1) const;
   bool PreciselyEqualTo(const Vector2& other) const;
};

LOCUS_GEOMETRY_API Vector2 operator+(const Vector2& v1, const Vector2& v2);
LOCUS_GEOMETRY_API Vector2 operator-(const Vector2& v1, const Vector2& v2);
LOCUS_GEOMETRY_API Vector2 operator*(const Vector2& v, float s);
LOCUS_GEOMETRY_API Vector2 operator*(float s, const Vector2& v);
LOCUS_GEOMETRY_API Vector2 operator/(const Vector2& v, float d);
LOCUS_GEOMETRY_API Vector2 operator-(const Vector2& v);

LOCUS_GEOMETRY_API bool operator==(const Vector2& v1, const Vector2& v2);
LOCUS_GEOMETRY_API bool operator!=(const Vector2& v1, const Vector2& v2);
LOCUS_GEOMETRY_API Vector2& operator+=(Vector2& v1, const Vector2& v2);
LOCUS_GEOMETRY_API Vector2& operator-=(Vector2& v1, const Vector2& v2);
LOCUS_GEOMETRY_API Vector2& operator*=(Vector2& v, float s);
LOCUS_GEOMETRY_API Vector2& operator/=(Vector2& v, float d);

}

namespace std
{

template <> struct hash<Locus::Vector2>
{
   std::size_t operator()(const Locus::Vector2& vec2D) const
   {
      return hash<float>()(vec2D.x) ^ hash<float>()(vec2D.y);
   }
};

}