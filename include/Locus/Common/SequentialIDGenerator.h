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
#include "IDType.h"

#include <mutex>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_COMMON_API SequentialIDGenerator
{
public:
   SequentialIDGenerator();

   ID_t NextID();
   void Reset();

private:
   ID_t nextID;
   std::mutex idMutex;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}