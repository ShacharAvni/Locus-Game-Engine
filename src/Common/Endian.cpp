/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Common/Endian.h"

#include <stdint.h>

namespace Locus
{

Endian SystemEndian()
{
   int16_t twoByteInt = 1;
   char* twoByteIntAsBytes = reinterpret_cast<char*>( &twoByteInt );

   return (twoByteIntAsBytes[0] == 1) ? Endian::Little : Endian::Big;
}

}