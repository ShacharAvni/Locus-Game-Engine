 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Common/ScopeFinalizer.h"

namespace Locus
{

ScopeFinalizer::ScopeFinalizer(const std::function<void()>& finalizeFunc)
   : finalizeFunc(finalizeFunc), cancelled(false)
{
}

void ScopeFinalizer::Cancel()
{
   cancelled = true;
}

ScopeFinalizer::~ScopeFinalizer()
{
   if (!cancelled)
   {
      finalizeFunc();
   }
}

}