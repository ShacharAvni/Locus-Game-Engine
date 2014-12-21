/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/XML/XMLTag.h"

#include <cstring>

namespace Locus
{

XMLTag* XMLTag::FindSubTag(const std::string& subTagName, std::size_t index)
{
   std::size_t finds = 0;

   for (XMLTag& subTag : subTags)
   {
      if (subTag.name == subTagName)
      {
         if (finds == index)
         {
            return &subTag;
         }

         ++finds;
      }
   }

   return nullptr;
}

const XMLTag* XMLTag::FindSubTag(const std::string& subTagName, std::size_t index) const
{
   return const_cast<XMLTag*>(this)->FindSubTag(subTagName, index);
}

XMLAttribute* XMLTag::FindAttribute(const std::string& attributeName, std::size_t index)
{
   std::size_t finds = 0;

   for (XMLAttribute& attribute : attributes)
   {
      if (attribute.name == attributeName)
      {
         if (finds == index)
         {
            return &attribute;
         }

         ++finds;
      }
   }

   return nullptr;
}

const XMLAttribute* XMLTag::FindAttribute(const std::string& attributeName, std::size_t index) const
{
   return const_cast<XMLTag*>(this)->FindAttribute(attributeName, index);
}

}