 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusCommonAPI.h"

#include <algorithm>
#include <type_traits>

#include <cstddef>

namespace Locus
{

enum class Endian
{
   Little,
   Big
};

LOCUS_COMMON_API Endian SystemEndian();

template <typename T>
class EndianNumber
{
public:
   static_assert(std::is_integral<T>::value, "T must be integral");

   EndianNumber(T num)
      : num(num)
   {
   }

   EndianNumber(char (&buffer)[sizeof(T)])
   {
      Set(buffer);
   }

   void Set(char (&buffer)[sizeof(T)])
   {
      char* numAsBytes = reinterpret_cast<char*>(&num);

      for (int byteIndex = 0; byteIndex < sizeof(T); ++byteIndex)
      {
         numAsBytes[byteIndex] = buffer[byteIndex];
      }
   }

   void ReverseEndian()
   {
      std::size_t midPoint = sizeof(T) / 2;

      char* numAsBytes = reinterpret_cast<char*>(&num);

      for (std::size_t byteOffset = 0; byteOffset < midPoint; ++byteOffset)
      {
         std::swap(numAsBytes[byteOffset], numAsBytes[sizeof(T) - 1 - byteOffset]);
      }
   }

   T num;
};

}