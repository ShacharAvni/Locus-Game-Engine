/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Math/SJTPermutations.h"

#include <algorithm>

#include <assert.h>

namespace Locus
{

SJTPermutations::SJTPermutations(int numElements)
   : numElements(numElements),
     elements(numElements),
     elementsIndexedByValue(numElements)
{
   assert(numElements >= 1);

   for (int elementIndex = 0; elementIndex < numElements; ++elementIndex)
   {
      elements[elementIndex] = std::make_unique<PermutationElement>();
      elements[elementIndex]->value = elementIndex;
      elements[elementIndex]->position = elementIndex;
      elements[elementIndex]->direction = Direction::Negative;

      elementsIndexedByValue[elementIndex] = elements[elementIndex].get();
   }

   elements[0]->direction = Direction::Zero;
}

int SJTPermutations::GetElement(unsigned int index) const
{
   assert(index < elements.size());

   return elements[index]->value;
}

std::size_t SJTPermutations::NumElements() const
{
   return numElements;
}

//{CodeReview:SJTPermutations}
bool SJTPermutations::GenerateNext()
{
   PermutationElement* largestWithNonZeroDirection = nullptr;

   for (int indexByValue = numElements - 1; indexByValue >= 0; --indexByValue)
   {
      if (elementsIndexedByValue[indexByValue]->direction != Direction::Zero)
      {
         largestWithNonZeroDirection = elementsIndexedByValue[indexByValue];
         break;
      }
   }

   if (largestWithNonZeroDirection == nullptr)
   {
      return false;
   }

   int currentPosition = largestWithNonZeroDirection->position;

   int swappedPosition, nextPositionInDirection;

   if (largestWithNonZeroDirection->direction == Direction::Positive)
   {
      swappedPosition = currentPosition + 1;
      nextPositionInDirection = swappedPosition + 1;
   }
   else
   {
      swappedPosition = currentPosition - 1;
      nextPositionInDirection = swappedPosition - 1;
   }

   std::swap(elements[swappedPosition], elements[currentPosition]);

   elements[currentPosition]->position = currentPosition;
   elements[swappedPosition]->position = swappedPosition;

   if ( (swappedPosition == 0) || (swappedPosition == (numElements - 1)) || (elements[nextPositionInDirection]->value > elements[swappedPosition]->value) )
   {
      elements[swappedPosition]->direction = Direction::Zero;
   }

   for (int indexByValue = elements[swappedPosition]->value + 1; indexByValue < numElements; ++indexByValue)
   {
      if (elementsIndexedByValue[indexByValue]->position > swappedPosition)
      {
         elementsIndexedByValue[indexByValue]->direction = Direction::Negative;
      }
      else
      {
         elementsIndexedByValue[indexByValue]->direction = Direction::Positive;
      }
   }

   return true;
}

}