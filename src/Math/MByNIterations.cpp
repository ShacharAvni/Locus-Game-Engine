/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Math/MByNIterations.h"

namespace Locus
{

MByNIterations::MByNIterations(std::size_t numElements, std::size_t numIterationsPerElement)
   : numElements(numElements), numIterationsPerElement(numIterationsPerElement), elements(numElements)
{
}

std::size_t MByNIterations::NumElements() const
{
   return numElements;
}

std::size_t MByNIterations::GetElement(std::size_t index) const
{
   return elements[index];
}

void MByNIterations::Reset()
{
   elements.assign(numElements, 0);
}

bool MByNIterations::R_IncrementFrom(std::size_t indexOfElementToIncrement)
{
   elements[indexOfElementToIncrement] = ((elements[indexOfElementToIncrement] + 1) % numIterationsPerElement);

   if (elements[indexOfElementToIncrement] == 0)
   {
      if (indexOfElementToIncrement == 0)
      {
         Reset();
         return false;
      }

      return R_IncrementFrom(indexOfElementToIncrement - 1);
   }

   return true;
}

bool MByNIterations::GenerateNext()
{
   return R_IncrementFrom(numElements - 1);
}

}