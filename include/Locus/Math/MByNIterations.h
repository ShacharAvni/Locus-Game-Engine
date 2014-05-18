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

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_MATH_API MByNIterations
{
public:
   MByNIterations(std::size_t numElements, std::size_t numIterationsPerElement);

   std::size_t NumElements() const;
   std::size_t GetElement(std::size_t index) const;

   void Reset();

   bool GenerateNext();

private:
   std::size_t numElements;
   std::size_t numIterationsPerElement;
   std::vector<std::size_t> elements;

   bool R_IncrementFrom(std::size_t indexOfElementToIncrement);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}