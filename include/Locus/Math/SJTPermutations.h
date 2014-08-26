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

//implementation of the Steinhaus–Johnson–Trotter algorithm for
//generating permutations of alternating parity with Even's Speedup
class LOCUS_MATH_API SJTPermutations
{
public:
   SJTPermutations(int numElements);

   SJTPermutations(const SJTPermutations&) = delete;
   SJTPermutations& operator=(const SJTPermutations&) = delete;

   std::size_t NumElements() const;
   int GetElement(unsigned int index) const;

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

}