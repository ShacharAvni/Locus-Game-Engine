/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Common/Util.h"

#include <algorithm>

#include <cctype>

namespace Locus
{

int CaseInsensitiveCompare(const std::string& first, const std::string& second)
{
   std::size_t firstLength = first.length();
   std::size_t secondLength = second.length();

   std::size_t minSize = std::min(firstLength, secondLength);

   for (std::size_t index = 0; index < minSize; ++index)
   {
      int toLowerFirst = ::tolower(first[index]);
      int toLowerSecond = ::tolower(second[index]);

      if (toLowerFirst < toLowerSecond)
      {
         return -1;
      }
      else if (toLowerFirst > toLowerSecond)
      {
         return 1;
      }
   }

   if (firstLength < secondLength)
   {
      return -1;
   }
   else if (firstLength > secondLength)
   {
      return 1;
   }

   return 0;
}

}