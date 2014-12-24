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
#include <vector>

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

/*!
 * \brief Split the string str given a delimiter.
 *
 * \param[in] str the input string
 * \param[in] delimiter the delimiter to split the string
 * \param[out] tokens will contain the parts of the string
 * between the occurrences of the delimiter in the input
 * string in order. If the delimiter is not found, then
 * tokens will contain one token (the whole string).
 */
LOCUS_COMMON_API void SplitString(const std::string& str, const std::string& delimiter, std::vector<std::string>& tokens);

} // namespace Locus