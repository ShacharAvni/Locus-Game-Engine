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