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