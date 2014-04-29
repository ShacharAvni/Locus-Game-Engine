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

#if defined(LOCUS_GEOMETRY_DLL)

    #define LOCUS_GEOMETRY_API LOCUS_SHARED_EXPORTS
    #define LOCUS_GEOMETRY_API_AT_DEFINITION LOCUS_SHARED_EXPORTS_AT_DEFINITION

#elif defined(LOCUS_DLL)

   #define LOCUS_GEOMETRY_API LOCUS_SHARED_IMPORTS
   #define LOCUS_GEOMETRY_API_AT_DEFINITION

#else

   #define LOCUS_GEOMETRY_API
   #define LOCUS_GEOMETRY_API_AT_DEFINITION

#endif


#if defined(LOCUS_DLL) && !defined(LOCUS_GEOMETRY_DLL)
   #define LOCUS_GEOMETRY_SHARED_IMPORTS
#endif