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

#include <string>
#include <sstream>

namespace Locus
{

template <typename T>
bool IsType(const std::string& str)
{
   std::istringstream stream(str);

   T check;
   stream >> std::noskipws >> check;

   return !stream.fail() && stream.eof();
}

LOCUS_COMMON_API void RemoveWhitespaces(std::string& str);
LOCUS_COMMON_API void TrimString(std::string& str);
LOCUS_COMMON_API void ToLower(std::string& str);
LOCUS_COMMON_API void ToUpper(std::string& str);

}