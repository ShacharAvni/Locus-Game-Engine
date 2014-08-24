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

#include <string>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

/// Base class of all things thrown by Locus.
class LOCUS_COMMON_API Exception
{
public:
   Exception(const std::string& message);

   /// Describes what this Exception is.
   const std::string& Message() const;

private:
   std::string message;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

} // namespace Locus