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

#include <limits>
#include <type_traits>

#include <cstdint>
#include <cassert>

namespace Locus
{

#ifndef NDEBUG

template <typename To, typename From>
void LossyCastAssertHelperFromUnsigned(From valueToCast)
{
   To maxTo = std::numeric_limits<To>::max();

   assert(static_cast<std::uint64_t>(valueToCast) <= static_cast<std::uint64_t>(maxTo));
}

//From Unsigned to Unsigned
template <typename To, typename From>
void LossyCastAssertHelper(From valueToCast, std::false_type, std::false_type)
{
   LossyCastAssertHelperFromUnsigned<To, From>(valueToCast);
}

//From Unsigned to Signed
template <typename To, typename From>
void LossyCastAssertHelper(From valueToCast, std::false_type, std::true_type)
{
   LossyCastAssertHelperFromUnsigned<To, From>(valueToCast);
}

//From Signed to Unsigned
template <typename To, typename From>
void LossyCastAssertHelper(From valueToCast, std::true_type, std::false_type)
{
   To maxTo = std::numeric_limits<To>::max();

   assert((valueToCast >= 0) && (static_cast<std::uint64_t>(valueToCast) <= static_cast<std::uint64_t>(maxTo)));
}

//From Signed to Signed
template <typename To, typename From>
void LossyCastAssertHelper(From valueToCast, std::true_type, std::true_type)
{
   To lowestTo = std::numeric_limits<To>::lowest();
   To maxTo = std::numeric_limits<To>::max();

   assert( (static_cast<std::int64_t>(valueToCast) >= static_cast<std::int64_t>(lowestTo)) &&
           (static_cast<std::int64_t>(valueToCast) <= static_cast<std::int64_t>(maxTo)) );
}

#endif

/// Casts from one integral type to another, asserting when data is lost.
template <typename To, typename From>
To LossyCast(From valueToCast)
{
   static_assert(std::is_integral<From>::value && std::is_integral<To>::value, "From and To must be integral");
   static_assert(sizeof(From) <= 8 && sizeof(To) <= 8, "From and To must be at most 64 bits");

#ifndef NDEBUG
   LossyCastAssertHelper<To, From>(valueToCast, std::is_signed<From>(), std::is_signed<To>());
#endif

   return static_cast<To>(valueToCast);
}

} // namespace Locus