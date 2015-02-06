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

#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__))
   #define _WIN32
#endif

//STBI_BUILD_DLL is defined by the stb_image library when being built as DLLs
//STBI_DLL is defined by users of the stb_image library for linking with the DLL version of the stb_image library
#if defined(STBI_DLL) && defined(STBI_BUILD_DLL)
   
   #error "STBI_DLL and STBI_BUILD_DLL must not be defined at the same time"

#elif defined(_WIN32) && defined(STBI_BUILD_DLL)

    #define STBI_API __declspec(dllexport)

#elif defined(_WIN32) && defined(STBI_DLL)

   #define STBI_API _declspec(dllimport)

#else

   #define STBI_API

#endif