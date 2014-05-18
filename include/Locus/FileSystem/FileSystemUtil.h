/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "LocusFileSystemAPI.h"

#include "Locus/FileSystem/MountedFilePath.h"

#include <string>
#include <vector>

namespace Locus
{

LOCUS_FILE_SYSTEM_API std::string GetExePath();
LOCUS_FILE_SYSTEM_API void ReadWholeFile(const std::string& filePath, std::vector<char>& data);
LOCUS_FILE_SYSTEM_API void ReadWholeFile(const MountedFilePath& mountedFilePath, std::vector<char>& data);

}