/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Common/Parsing.h"

#include <algorithm>

#include <cctype>

namespace Locus
{

void RemoveWhitespaces(std::string& str)
{
   str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
}

void TrimString(std::string& str)
{
   std::istringstream trimmerStream(str);
   trimmerStream >> str;
}

void ToLower(std::string& str)
{
   std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void ToUpper(std::string& str)
{
   std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void TrimUpToLastOccurenceOfChar(std::string& str, char c)
{
   std::size_t indexOfLastC = str.rfind(c);

   if (indexOfLastC != std::string::npos)
   {
      str = str.substr(0, indexOfLastC + 1);
   }
}

void SplitString(const std::string& str, const std::string& delimiter, std::vector<std::string>& tokens)
{
   tokens.clear();

   std::size_t indexTo = str.find(delimiter);

   if (indexTo != std::string::npos)
   {
      std::size_t indexFrom = 0;

      do
      {
         tokens.push_back(str.substr(indexFrom, indexTo - indexFrom));

         indexFrom = indexTo + 1;

         indexTo = str.find(delimiter, indexFrom);
      } while (indexTo != std::string::npos);

      std::size_t length = str.length();

      if (indexFrom == length)
      {
         tokens.push_back("");
      }
      else
      {
         tokens.push_back(str.substr(indexFrom, length - indexFrom));
      }
   }
   else
   {
      tokens.push_back(str);
   }
}

bool EndsWith(const std::string& str, const std::string& ending)
{
   if (str.length() >= ending.length())
   {
      return (str.compare(str.length() - ending.length(), ending.length(), ending) == 0);
   }

   return false;
}

bool StartsWith(const std::string& str, const std::string& starting)
{
   if (str.length() >= starting.length())
   {
      return (str.compare(0, starting.length(), starting) == 0);
   }

   return false;
}

}