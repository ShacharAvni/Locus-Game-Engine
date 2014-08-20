/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Common/Random.h"

namespace Locus
{

Random::Random()
   : generator( randomDevice() )
{
}

double Random::RandomDouble(double begin, double end)
{
   std::uniform_real_distribution<> realDistribution(begin, end);
   return realDistribution(generator);
}

int Random::RandomInt(int begin, int end)
{
   std::uniform_int_distribution<> intDistribution(begin, end);
   return intDistribution(generator);
}

bool Random::FlipCoin(double probability)
{
   return (RandomDouble(0, 1) <= probability);
}

}