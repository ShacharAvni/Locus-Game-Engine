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

#include "Locus/Preprocessor/CompilerDefinitions.h"

#if defined(LOCUS_AUDIO_DLL)

    #define LOCUS_AUDIO_API LOCUS_SHARED_EXPORTS

#elif defined(LOCUS_DLL)

   #define LOCUS_AUDIO_API LOCUS_SHARED_IMPORTS

#else

   #define LOCUS_AUDIO_API

#endif


#if defined(LOCUS_DLL) && !defined(LOCUS_AUDIO_DLL)
   #define LOCUS_AUDIO_SHARED_IMPORTS
#endif