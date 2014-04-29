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

#include "LocusFileSystemAPI.h"

#include <string>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

struct LOCUS_FILE_SYSTEM_API MountedFilePath
{
   explicit MountedFilePath(const char* path);
   explicit MountedFilePath(const std::string& path);

   std::string path;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

LOCUS_FILE_SYSTEM_API MountedFilePath operator +(const MountedFilePath& first, const MountedFilePath& second);
LOCUS_FILE_SYSTEM_API MountedFilePath operator +(const MountedFilePath& first, const std::string& second);

}