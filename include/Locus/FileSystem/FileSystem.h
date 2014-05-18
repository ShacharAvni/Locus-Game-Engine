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

#include "LocusFileSystemAPI.h"

#include <string>

namespace Locus
{

class LOCUS_FILE_SYSTEM_API FileSystem
{
public:
   FileSystem(const char* argv0);
   ~FileSystem();
};

LOCUS_FILE_SYSTEM_API void MountDirectoryOrArchive(const std::string& fullPath);

}