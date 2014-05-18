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

namespace Locus
{

typedef unsigned int ID_t;
LOCUS_COMMON_API extern const ID_t BAD_ID;

class LOCUS_COMMON_API IDGenerator
{
public:
   virtual ~IDGenerator();

   virtual ID_t NextID() = 0;
   virtual void Reset() = 0;
};

}