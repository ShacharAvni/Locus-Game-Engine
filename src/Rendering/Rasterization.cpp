/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/Rasterization.h"

#include "Locus/Math/Vectors.h"

#include <cmath>

namespace Locus
{

void Bresenham2D(const IVector2& pixelDifference, std::vector<IVector2>& pixelOffsets, std::size_t& numPixelsWritten)
{
   Locus::IVector2 step1(0, 0);
   Locus::IVector2 step2(0, 0);

   if (pixelDifference.x < 0)
   {
      step1.x = -1;
      step2.x = -1;
   }
   else if (pixelDifference.x > 0)
   {
      step1.x = 1;
      step2.x = 1;
   }

   if (pixelDifference.y < 0)
   {
      step1.y = -1;
   }
   else if (pixelDifference.y > 0)
   {
      step1.y = 1;
   }

   int longest = std::abs(pixelDifference.x);
   int shortest = std::abs(pixelDifference.y);

   if (longest <= shortest)
   {
      std::swap(longest, shortest);

      if (pixelDifference.y < 0)
      {
         step2.y = -1;
      }
      else if (pixelDifference.y > 0)
      {
         step2.y = 1;
      }

      step2.x = 0;            
   }

   std::size_t numPixels = (longest + 1);

   if (numPixels > pixelOffsets.size())
   {
      pixelOffsets.resize(numPixels);
   }

   int numerator = (longest >> 1);

   Locus::IVector2 pixelOffset(0, 0);

   for (std::size_t pixelIndex = 0; pixelIndex < numPixels; ++pixelIndex)
   {
      pixelOffsets[pixelIndex] = pixelOffset;

      numerator += shortest;

      if (numerator >= longest)
      {
         numerator -= longest;

         pixelOffset += step1;
      }
      else
      {
         pixelOffset += step2;
      }
   }

   numPixelsWritten = numPixels;
}

}