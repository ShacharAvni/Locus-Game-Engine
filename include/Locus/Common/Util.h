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

#include <vector>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <string>

namespace Locus
{

namespace Util
{

template <class T>
static void GetUniqueItems(const std::vector<T>& items, std::vector<T>& uniqueItems, std::function<bool(const T&, const T&)> compare, std::unordered_map<std::size_t, std::size_t>& uniqueItemMap)
{
   struct SortedItem
   {
      SortedItem()
         : index(0)
      {
      }

      SortedItem(const T& item, std::size_t index)
         : item(item), index(index)
      {
      }

      bool operator<(const SortedItem& other) const
      {
         return item < other.item;
      }

      T item;
      std::size_t index;
   };

   std::size_t numItems = items.size();

   std::vector<SortedItem> sortedItems(numItems);
   for (std::size_t itemIndex = 0; itemIndex < numItems; ++itemIndex)
   {
      sortedItems[itemIndex].item = items[itemIndex];
      sortedItems[itemIndex].index = itemIndex;
   }

   std::sort(sortedItems.begin(), sortedItems.end());

   //extract unique items

   std::size_t itemFinalIndex = 0;
   T checkItem = sortedItems[0].item;
   uniqueItems.push_back(checkItem);

   uniqueItemMap[sortedItems[0].index] = 0;

   for (std::size_t i = 1; i < numItems; ++i) 
   {
      uniqueItemMap[sortedItems[i].index] = itemFinalIndex;

      if (!compare(sortedItems[i].item, checkItem))
      {
         ++itemFinalIndex;
         uniqueItemMap[sortedItems[i].index] = itemFinalIndex;

         checkItem = sortedItems[i].item;
         uniqueItems.push_back(checkItem);
      } 
   }
}

template <class T>
static void GetUniqueItems(const std::vector<T>& items, std::vector<T>& uniqueItems, std::unordered_map<std::size_t,std::size_t>& uniqueItemMap)
{
   return GetUniqueItems<T>(items, uniqueItems, [](const T& first, const T& second)->bool{ return first == second; }, uniqueItemMap);
}

template <class T, class Compare>
void InsertionSort(std::vector<T>& sortedInterval, Compare comp)
{
   for (std::size_t i = 1, numElements = sortedInterval.size(); i < numElements; ++i)
   {
      if (!comp(sortedInterval[i - 1], sortedInterval[i]))
      {
         typename std::vector<T>::iterator whereToPlace = std::lower_bound(sortedInterval.begin(), sortedInterval.begin() + i - 1, sortedInterval[i], comp);
         std::size_t whereToPlaceIndex = whereToPlace - sortedInterval.begin();

         for (std::size_t swapIndex = whereToPlaceIndex; swapIndex < i; ++swapIndex)
         {
            std::swap(sortedInterval[swapIndex], sortedInterval[i]);
         }
      }
   }
}

template <class T>
void ClearAndShrink(std::vector<T>& vec)
{
   vec.clear();
   vec.shrink_to_fit();
}

template <class T>
void ClearStack(std::stack<T>& stack)
{
   while (!stack.empty())
   {
      stack.pop();
   }
}

}

}