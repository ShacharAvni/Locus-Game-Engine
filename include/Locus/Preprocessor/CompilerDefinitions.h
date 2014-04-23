 /********************************************************************************************************************************************************\
 *                                                                                                                                                        *
 *   This file is part of the Locus Game Engine                                                                                                           *
 *                                                                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                *
 *                                                                                                                                                        *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),   *
 *   to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the   *
 *   Software is furnished to do so, subject to the following conditions:                                                                                 *
 *                                                                                                                                                        *
 *   a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                      *
 *   b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the             *
 *      documentation and/or other materials provided with the distribution.                                                                              *
 *                                                                                                                                                        *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    *
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,            *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF            *
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,    *
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                   *
 *                                                                                                                                                        *
 \********************************************************************************************************************************************************/

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
