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

#include <random>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

/*!
 * \brief Generates pseudo-random numbers.
 *
 * \details Wraps a std::mt19937 random generator.
 */
class LOCUS_COMMON_API Random
{
public:
   Random();

   /// Generates a random double between begin and end inclusively.
   double RandomDouble(double begin, double end);

   /// Generates a random integer between begin and end inclusively.
   int RandomInt(int begin, int end);

   /*!
    * \return true with the given probability.
    *
    * \param[in] probability Must be between zero and one inclusively.
    */
   bool FlipCoin(double probability);

private:
   std::random_device randomDevice;
   std::mt19937 generator;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

} // namespace Locus