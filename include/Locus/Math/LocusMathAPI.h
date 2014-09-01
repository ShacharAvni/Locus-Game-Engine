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

#include "Locus/Preprocessor/CompilerDefinitions.h"

#if defined(LOCUS_MATH_SHARED)

    #define LOCUS_MATH_API LOCUS_SHARED_EXPORTS

#elif defined(LOCUS_SHARED)

   #define LOCUS_MATH_API LOCUS_SHARED_IMPORTS

#else

   #define LOCUS_MATH_API

#endif


#if defined(LOCUS_SHARED) && !defined(LOCUS_MATH_SHARED)

   #define LOCUS_MATH_SHARED_IMPORTS

#endif