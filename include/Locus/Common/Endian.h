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

#include "LocusCommonAPI.h"

#include <algorithm>
#include <type_traits>

#include <cstddef>

namespace Locus
{

/// CPU endianness
enum class Endian
{
   Little,
   Big
};

/// Determines the system's CPU endianness.
LOCUS_COMMON_API Endian SystemEndian();

/*!
 * An integral number whose endianness can be swapped.
 */
//TODO: Add support for float, double, and long double
template <typename T>
class EndianNumber
{
public:
   static_assert(std::is_integral<T>::value, "T must be integral");

   /// Initialize with the given value in the current system endianness.
   EndianNumber(T num)
      : num(num)
   {
   }

   /*!
    * \brief Initialize with the given array of bytes.
    *
    * \sa Set
    *
    * The number is initialized using the Set method.
    */
   EndianNumber(char (&buffer)[sizeof(T)])
   {
      Set(buffer);
   }

   /*!
    * \brief Set the number using the given array of bytes.
    *
    * \param buffer array of bytes with length the same as
    * sizeof(T) where T is the template argument type.
    *
    * The array is used naively, with buffer[0] used as the
    * highest order byte and buffer[sizeof(T) - 1] being the
    * lowest order byte.
    */
   void Set(char (&buffer)[sizeof(T)])
   {
      char* numAsBytes = reinterpret_cast<char*>(&num);

      for (std::size_t byteIndex = 0; byteIndex < sizeof(T); ++byteIndex)
      {
         numAsBytes[byteIndex] = buffer[byteIndex];
      }
   }

   /*!
    * \brief Swap the bytes of the number so that it is expressed in
    * the alternate endianness.
    */
   void ReverseEndian()
   {
      std::size_t midPoint = sizeof(T) / 2;

      char* numAsBytes = reinterpret_cast<char*>(&num);

      for (std::size_t byteOffset = 0; byteOffset < midPoint; ++byteOffset)
      {
         std::swap(numAsBytes[byteOffset], numAsBytes[sizeof(T) - 1 - byteOffset]);
      }
   }

   /// The underlying integral number.
   T num;
};

} // namespace Locus