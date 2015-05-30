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

#include <vector>
#include <stack>
#include <algorithm>
#include <functional>
#include <string>

namespace Locus
{

/*!
 * \brief Get the unique items in a std::vector and an index mapping, given a comparator.
 *
 * \param[in] items The input items. The class T must have a supporting operator < implementation.
 * Can be empty.
 *
 * \param[out] uniqueItems The unique items in the items vector.
 *
 * \param[in] compare The comparator to use. Should return true if the two values are considered equal.
 *
 * \param[out] uniqueIndices An index mapping from the original indices to the indices in the unique
 * vector. uniqueIndices[i] will hold the index in the unique vector where items[i] maps to. i must
 * be between zero and items.size() - 1 inclusively.
 */
template <class T>
void GetUniqueItems(const std::vector<T>& items, std::vector<T>& uniqueItems, std::function<bool(const T&, const T&)> compare, std::vector<std::size_t>& uniqueIndices)
{
   struct SortedItem
   {
      SortedItem()
         : item(nullptr), index(0)
      {
      }

      bool operator<(const SortedItem& other) const
      {
         return *item < *(other.item);
      }

      const T* item;
      std::size_t index;
   };

   std::size_t numItems = items.size();

   uniqueItems.clear();
 
   uniqueIndices.resize(numItems);
  
   if (numItems > 0)
   {
      //sort the items while storing their original indices

      std::vector<SortedItem> sortedItems(numItems);
      for (std::size_t itemIndex = 0; itemIndex < numItems; ++itemIndex)
      {
         sortedItems[itemIndex].item = &items[itemIndex];
         sortedItems[itemIndex].index = itemIndex;
      }

      std::sort(sortedItems.begin(), sortedItems.end());

      //extract unique items

      const T* checkItem = sortedItems[0].item;

      uniqueItems.push_back(*checkItem);

      std::size_t itemFinalIndex = 0;

      uniqueIndices[sortedItems[0].index] = 0;

      for (std::size_t i = 1; i < numItems; ++i) 
      {
         uniqueIndices[sortedItems[i].index] = itemFinalIndex;

         if (! compare(*(sortedItems[i].item), *checkItem) )
         {
            ++itemFinalIndex;
            uniqueIndices[sortedItems[i].index] = itemFinalIndex;

            checkItem = sortedItems[i].item;
            uniqueItems.push_back(*checkItem);
         } 
      }
   }
}

/*!
 * \brief Calls the other GetUniqueItems overload with a comparator that uses the == operator of the given
 * template type.
 *
 * \sa template <class T>
 *     void GetUniqueItems(const std::vector<T>& items, std::vector<T>& uniqueItems, std::function<bool(const T&, const T&)> compare, std::vector<std::size_t>& uniqueIndices)
 */
template <class T>
void GetUniqueItems(const std::vector<T>& items, std::vector<T>& uniqueItems, std::vector<std::size_t>& uniqueIndices)
{
   return GetUniqueItems<T>(items, uniqueItems, [](const T& first, const T& second)->bool{ return first == second; }, uniqueIndices);
}

/*!
 * \brief The items are sorted (using the default comparator) and then consecutive
 * duplicates are removed using the given equality comparison. If two consecutive
 * elements after the sort are considered equal, then the first element is kept.
 */
template <class T, class EqualityCompare>
void SortAndRemoveDuplicates(std::vector<T>& items, const EqualityCompare& equalityCompare)
{
   std::sort(items.begin(), items.end());

   auto uniqueResult = std::unique(items.begin(), items.end(), equalityCompare);

   items.resize( std::distance(items.begin(), uniqueResult) );
}

/*!
 * \brief The other overload of SortAndRemoveDuplicates is called given operator==
 * as the equality comparison.
 *
 * \sa template <class T, class EqualityCompare>
 *     void SortAndRemoveDuplicates(std::vector<T>& items, const EqualityCompare& equalityCompare)
 */
template <class T>
void SortAndRemoveDuplicates(std::vector<T>& items)
{
   SortAndRemoveDuplicates(items, [](const T& first, const T& second)->bool{ return first == second; });
}

/*!
 * \brief Sorts the input vector using Insertion Sort.
 *
 * \param[in,out] sortedInterval The vector to sort.
 *
 * \param[in] comp The comparator to use. Must be compatible with the comparators
 * passed to standard library functions such as std::sort.
 */
template <class T, class Compare>
void InsertionSort(std::vector<T>& sortedInterval, Compare comp)
{
   for (std::size_t i = 1, numElements = sortedInterval.size(); i < numElements; ++i)
   {
      if (!comp(sortedInterval[i - 1], sortedInterval[i]))
      {
         auto whereToPlace = std::lower_bound(sortedInterval.begin(), sortedInterval.begin() + i - 1, sortedInterval[i], comp);
         std::size_t whereToPlaceIndex = whereToPlace - sortedInterval.begin();

         for (std::size_t swapIndex = whereToPlaceIndex; swapIndex < i; ++swapIndex)
         {
            std::swap(sortedInterval[swapIndex], sortedInterval[i]);
         }
      }
   }
}

/// Calls clear() then shrink_to_fit() on the given vector.
template <class T>
void ClearAndShrink(std::vector<T>& vec)
{
   vec.clear();
   vec.shrink_to_fit();
}

/// Clears the given stack.
template <class T>
void ClearStack(std::stack<T>& stack)
{
   while (!stack.empty())
   {
      stack.pop();
   }
}

/// Clamps the value to between min and max exclusively.
template <class T>
T Clamp(T value, T min, T max)
{
   return value < min ? min : (value > max ? max : value);
}

/*!
 * \brief Case insensitive lexicographic comparison.
 *
 * \return negative if first comes before second, zero
 * if first and second compare the same, or positive if first
 * comes after second.
 */
LOCUS_COMMON_API int CaseInsensitiveCompare(const std::string& first, const std::string& second);

namespace ArrayLengthHelper
{

template <std::size_t N>
struct TypeOfSize
{
      typedef char type[N];
};

template <typename T, std::size_t Size>
typename TypeOfSize<Size>::type& GetSize(T(&)[Size]);

}

} // namespace Locus

//courtesy of: http://stackoverflow.com/questions/3368883/how-does-this-size-of-array-template-function-work
#define LOCUS_ARRAY_LENGTH(anArray) sizeof(Locus::ArrayLengthHelper::GetSize(anArray))