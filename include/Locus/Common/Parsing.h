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

#include <string>
#include <sstream>

namespace Locus
{

/*!
 * \brief Returns true if the given std::string can be converted to
 * the given type.
 *
 * \details There should be no whitespace in front.
 *
 * Examples:
 * IsType<int>("5") returns true.
 * IsType<int>(" 5") returns false.
 * IsType<int>("5a") returns false.
 * IsType<float>("6.0") returns true.
 */
template <typename T>
bool IsType(const std::string& str)
{
   std::istringstream stream(str);

   T check;
   stream >> std::noskipws >> check;

   return !stream.fail() && stream.eof();
}

/// Removes all whitespace characters.
LOCUS_COMMON_API void RemoveWhitespaces(std::string& str);

/// Removes all leading and trailing whitespace characters.
LOCUS_COMMON_API void TrimString(std::string& str);

/// Calls std::tolower on all characters.
LOCUS_COMMON_API void ToLower(std::string& str);

/// Calls std::toupper on all characters.
LOCUS_COMMON_API void ToUpper(std::string& str);

/*!
 * \brief str becomes the characters after the last occurrence of c.
 *
 * \details If c is not in str, then str is untouched.
 */
LOCUS_COMMON_API void TrimUpToLastOccurenceOfChar(std::string& str, char c);

} // namespace Locus