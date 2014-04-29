 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusCommonAPI.h"

#include <functional>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_COMMON_API ScopeFinalizer
{
public:
   ScopeFinalizer(const std::function<void()>& finalizeFunc);
   ~ScopeFinalizer();

   void Cancel();

private:
   std::function<void()> finalizeFunc;
   bool cancelled;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}