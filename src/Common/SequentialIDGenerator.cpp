/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Common/SequentialIDGenerator.h"

#include <limits>

namespace Locus
{

SequentialIDGenerator::SequentialIDGenerator()
   : nextID(1)
{
}

ID_t SequentialIDGenerator::NextID()
{
   idMutex.lock();

   ID_t nextIDToReturn = nextID;

   if (nextID != BAD_ID)
   {
      ++nextID;

      if (nextID == std::numeric_limits<ID_t>::max())
      {
         nextID = BAD_ID;
      }
   }

   idMutex.unlock();

   return nextIDToReturn;
}

void SequentialIDGenerator::Reset()
{
   idMutex.lock();

   nextID = 1;

   idMutex.unlock();
}

}