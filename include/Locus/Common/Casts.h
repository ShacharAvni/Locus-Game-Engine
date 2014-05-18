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

template <typename From, typename To>
void LossyCastAssertHelperFromUnsigned(From valueToCast)
{
   To maxTo = std::numeric_limits<To>::max();

   assert(static_cast<std::uint64_t>(valueToCast) <= static_cast<std::uint64_t>(maxTo));
}

//From Unsigned to Unsigned
template <typename From, typename To>
void LossyCastAssertHelper(From valueToCast, std::false_type, std::false_type)
{
   LossyCastAssertHelperFromUnsigned<From, To>(valueToCast);
}

//From Unsigned to Signed
template <typename From, typename To>
void LossyCastAssertHelper(From valueToCast, std::false_type, std::true_type)
{
   LossyCastAssertHelperFromUnsigned<From, To>(valueToCast);
}

//From Signed to Unsigned
template <typename From, typename To>
void LossyCastAssertHelper(From valueToCast, std::true_type, std::false_type)
{
   To maxTo = std::numeric_limits<To>::max();

   assert((valueToCast >= 0) && (static_cast<std::uint64_t>(valueToCast) <= static_cast<std::uint64_t>(maxTo)));
}

//From Signed to Signed
template <typename From, typename To>
void LossyCastAssertHelper(From valueToCast, std::true_type, std::true_type)
{
   To lowestTo = std::numeric_limits<To>::lowest();
   To maxTo = std::numeric_limits<To>::max();

   assert( (static_cast<std::int64_t>(valueToCast) >= static_cast<std::int64_t>(lowestTo)) &&
           (static_cast<std::int64_t>(valueToCast) <= static_cast<std::int64_t>(maxTo)) );
}

#endif

template <typename From, typename To>
To LossyCast(From valueToCast)
{
   static_assert(std::is_integral<From>::value && std::is_integral<To>::value, "From and To must be integral");

#ifndef NDEBUG
   LossyCastAssertHelper<From, To>(valueToCast, std::is_signed<From>(), std::is_signed<To>());
#endif

   return static_cast<To>(valueToCast);
}

}