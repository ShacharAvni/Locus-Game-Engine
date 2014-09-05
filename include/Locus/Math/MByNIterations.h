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

/*!
 * \brief Used for making M variables that
 * each increment from 0 to N-1 as if they
 * were being incremented in nested loops.
 *
 * \example MByNIterationsExample.cpp
 */
class LOCUS_MATH_API MByNIterations
{
public:
   MByNIterations(std::size_t numElements, std::size_t numIterationsPerElement);

   /// \return The number of elements (can be thought of as the number of nested loops).
   std::size_t NumElements() const;

   /// \return The value of the element at the given index.
   std::size_t GetElement(std::size_t index) const;

   /// Sets all elements back to zero.
   void Reset();

   /*!
    * \brief Sets all the elements to have the value
    * at the next iteration.
    *
    * \return false if all the elements are in the final
    * state.
    */
   bool GenerateNext();

private:
   std::size_t numElements;
   std::size_t numIterationsPerElement;
   std::vector<std::size_t> elements;

   bool R_IncrementFrom(std::size_t indexOfElementToIncrement);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

} // namespace Locus