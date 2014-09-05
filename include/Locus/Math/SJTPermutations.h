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

#include "LocusMathAPI.h"

#include <vector>
#include <memory>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

//{CodeReview:SJTPermutations}
/*!
 * \brief An implementation of the Steinhaus-Johnson-Trotter
 * algorithm for generating permutations of alternating parity
 * with Even's Speedup.
 *
 * \details The elements are stored as the integers [0, N-1]
 * and are initially in the following order: {0, 1, 2, ..., N - 1}.
 *
 * \example SJTPermutationsExample.cpp
 */
class LOCUS_MATH_API SJTPermutations
{
public:
   SJTPermutations(int numElements);

   SJTPermutations(const SJTPermutations&) = delete;
   SJTPermutations& operator=(const SJTPermutations&) = delete;

   /// \return The number of integers in the permutations.
   std::size_t NumElements() const;

   /// \return The integer at the given index.
   int GetElement(unsigned int index) const;

   //{CodeReview:SJTPermutations}
   /*!
    * \brief Generates the next permutation.
    *
    * \return false if there are no more
    * permutations.
    */
   bool GenerateNext();

private:
   enum class Direction
   {
      Zero,
      Positive,
      Negative
   };

   struct PermutationElement
   {
      int value;
      int position;
      Direction direction;
   };

   int numElements;

   std::vector<std::unique_ptr<PermutationElement>> elements;
   std::vector<PermutationElement*> elementsIndexedByValue;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

} // namespace Locus