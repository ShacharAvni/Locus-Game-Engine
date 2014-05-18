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

#if defined(_WIN32) || defined(__WIN32__) || defined( __MINGW32__) || defined(__CYGWIN__)
   #define LOCUS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
   #define LOCUS_OSX
#elif defined(__linux__)
   #define LOCUS_LINUX
#else
   #warning "Unsupported architecture"
#endif

#if defined(LOCUS_WINDOWS)
   #define LOCUS_SHARED_IMPORTS __declspec(dllimport)
   #define LOCUS_SHARED_EXPORTS __declspec(dllexport)
   #define LOCUS_SHARED_EXPORTS_AT_DEFINITION __declspec(dllexport)
#elif defined(LOCUS_OSX)
   #define LOCUS_SHARED_IMPORTS
   #define LOCUS_SHARED_EXPORTS
   #define LOCUS_SHARED_EXPORTS_AT_DEFINITION
#elif defined(LOCUS_LINUX)
   #ifdef __GNUC__
      #define LOCUS_SHARED_IMPORTS
      #define LOCUS_SHARED_EXPORTS __attribute__ ((visibility ("default")))
      #define LOCUS_SHARED_EXPORTS_AT_DEFINITION
   #endif
#else
   #define LOCUS_SHARED_IMPORTS
   #define LOCUS_SHARED_EXPORTS
   #define LOCUS_SHARED_EXPORTS_AT_DEFINITION
#endif
