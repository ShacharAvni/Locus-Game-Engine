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

class LOCUS_COMMON_API Random
{
public:
   Random();

   double randomDouble(double begin, double end);
   int randomInt(int begin, int end);
   bool flipCoin(double p);

private:
   std::random_device randomDevice;
   std::mt19937 generator;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}