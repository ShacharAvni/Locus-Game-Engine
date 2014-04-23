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

#include "Locus/Math/SJTPermutations.h"

#include <algorithm>

#include <assert.h>

namespace Locus
{

SJTPermutations::SJTPermutations(int numElements)
   : numElements(numElements), elements(numElements), elementsIndexedByValue(numElements)
{
   assert(numElements >= 1);

   for (int elementIndex = 0; elementIndex < numElements; ++elementIndex)
   {
      elements[elementIndex].reset(new PermutationElement());
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