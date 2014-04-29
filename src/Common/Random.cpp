 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Common/Random.h"

namespace Locus
{

Random::Random()
   : generator( randomDevice() )
{
}

//these functions get a random number in a given interval (between "begin" and "end" inclusively)
double Random::randomDouble(double begin, double end)
{
   std::uniform_real_distribution<> realDistribution(begin, end);
   return realDistribution(generator);
}

int Random::randomInt(int begin, int end)
{
   std::uniform_int_distribution<> intDistribution(begin, end);
   return intDistribution(generator);
}

//this function flips a coin that lands heads with probability p.
//true = heads, false = tails...or vice-versa
bool Random::flipCoin(double p)
{
   return (randomDouble(0, 1) <= p);
}

}