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

bool operator==(const TextureCoordinate& coord1, const TextureCoordinate& coord2)
{
   return (Float::FEqual<float>(coord1.x, coord2.x) && Float::FEqual<float>(coord1.y, coord2.y));
}

bool operator <(const TextureCoordinate& coord1, const TextureCoordinate& coord2)
{
   if (coord1.x == coord2.x)
   {
      if (coord1.y == coord2.y)
      {
         return false;
      }
      else
      {
         return coord1.y < coord2.y;
      }
   }
   else
   {
      return coord1.x < coord2.x;
   }
}

}