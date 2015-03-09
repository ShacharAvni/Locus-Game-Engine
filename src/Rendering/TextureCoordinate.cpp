/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/TextureCoordinate.h"

#include "Locus/Common/Float.h"

#include <cassert>

namespace Locus
{

TextureCoordinate::TextureCoordinate()
   : x(0), y(0)
{
}

TextureCoordinate::TextureCoordinate(float x, float y)
   : x(x), y(y)
{
}

void TextureCoordinate::SerializeTo(float* destination) const
{
   assert(destination != nullptr);

   destination[0] = x;
   destination[1] = y;
}

TextureCoordinate operator+(const TextureCoordinate& t1, const TextureCoordinate& t2)
{
   return TextureCoordinate(t1.x + t2.x, t1.y + t2.y);
}

TextureCoordinate& operator+=(TextureCoordinate& t1, const TextureCoordinate& t2)
{
   t1.x += t2.x;
   t1.y += t2.y;

   return t1;
}

TextureCoordinate operator-(const TextureCoordinate& t1, const TextureCoordinate& t2)
{
   return TextureCoordinate(t1.x - t2.x, t1.y - t2.y);
}

TextureCoordinate& operator-=(TextureCoordinate& t1, const TextureCoordinate& t2)
{
   t1.x -= t2.x;
   t1.y -= t2.y;

   return t1;
}

TextureCoordinate operator/(const TextureCoordinate& t1, float k)
{
   return TextureCoordinate(t1.x / k, t1.y / k);
}

TextureCoordinate& operator/=(TextureCoordinate& t1, float k)
{
   t1.x /= k;
   t1.y /= k;

   return t1;
}

TextureCoordinate operator*(const TextureCoordinate& t1, float k)
{
   return TextureCoordinate(t1.x * k, t1.y * k);
}

TextureCoordinate operator*(float k, const TextureCoordinate& t1)
{
   return (t1 * k);
}
TextureCoordinate& operator*=(TextureCoordinate& t1, float k)
{
   t1.x *= k;
   t1.y *= k;

   return t1;
}

bool operator==(const TextureCoordinate& coord1, const TextureCoordinate& coord2)
{
   return ( FEqual<float>(coord1.x, coord2.x) && FEqual<float>(coord1.y, coord2.y) );
}

bool operator <(const TextureCoordinate& coord1, const TextureCoordinate& coord2)
{
   if (coord1.x == coord2.x)
   {
      return coord1.y < coord2.y;
   }
   else
   {
      return coord1.x < coord2.x;
   }
}

}